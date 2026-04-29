#include "Flock.hpp"

#include <random>

#include "Core/JobSystem/Awaitables/Primitives/ParallelForChunk.hpp"
#include "Core/Maths/Utility.hpp"
#include "Core/Time/Clock.hpp"
#include "Maths/Easing.hpp"

USING_RUKEN_NAMESPACE

Flock::Flock(RkSize const in_size, RenderDevice& in_device):
	m_boids                  {in_size},
	m_positions              {in_size, Constants<Vector3m>::zero},
	m_velocities             {in_size},
	m_accelerations          {in_size, Constants<Vector3m>::zero},
	m_cohesion_hash_map      {in_size},
	m_avoidance_hash_map     {in_size},
	m_flock_heading          {in_size},
	m_avoidance_heading      {in_size},
	m_flock_center           {in_size},
	m_number_flockmates      (in_size),
	m_render_matrices        {in_size}
{
	std::random_device 			   device 	    {};
	std::mt19937	   			   random 	    {device()};
	std::uniform_real_distribution distribution {-1.0f, 1.0f};

	for (RkSize index {0uz}; index < m_boids; index++)
	{
		m_velocities[index] = Vector3m(
			static_cast<Meters>(distribution(random)),
			static_cast<Meters>(distribution(random)),
			static_cast<Meters>(distribution(random))
		) * (minimal_boid_speed + maximal_boid_speed) / 2.0f;

		m_positions[index] = Vector3m(
			static_cast<Meters>(distribution(random)),
			static_cast<Meters>(distribution(random)),
			static_cast<Meters>(distribution(random))
		) * 10.0f;
	}
}

AsyncTask<ECSJobQueue> Flock::Update(ServiceProvider const& in_service_provider) noexcept
{
	auto const delta_time {in_service_provider.LocateService<Clock>()->LastTickDuration()};

	// Applying intents
	co_await ApplyCohesionIntent  ();
	co_await ApplySeparationIntent();
	co_await ApplyTargetIntent	  ();
	co_await ApplyHeightIntent    (delta_time);

	// Moving boids
	co_await MoveBoids(delta_time);

	// Send transforms to renderer
	co_await ApplyTransform();
}

RkVoid Flock::UpdateCohesionStatsForIndex(RkSize const in_boid, RkUint32 const in_hash, RkUint32 const in_hash_index)
{
	int total = 0;
	for (int other = in_hash_index; other < m_boids && m_cohesion_hash_map.hash_map[other].hash == in_hash; other++)
	{
		// No more than 3 boids taken into account per cell should be a good enough approximation
		// We could even try to sample randomly for a better distribution.
		if (total++ >= 3)
			break;

		RkUint32 const other_index {m_cohesion_hash_map.hash_map[other].entity_index};
		Vector3m const offset      {m_positions[other_index] - m_positions[in_boid]};

		if (offset.SquareLength() > cohesion_distance * cohesion_distance)
			continue;

		Vector3m velocity {m_velocities[other_index]};

		m_number_flockmates[in_boid]++;
		m_flock_heading    [in_boid] += velocity / velocity.SquareLength();
		m_flock_center     [in_boid] += m_positions[other_index];
	}
}

RkVoid Flock::UpdateAvoidanceStatsForIndex(RkSize const in_boid, RkUint32 const in_hash, RkUint32 const in_hash_index)
{
	int total = 0;
	for (int other = in_hash_index; other < m_boids && m_avoidance_hash_map.hash_map[other].hash == in_hash; other++)
	{
		// No more than 3 boids taken into account per cell should be a good enough approximation
		// We could even try to sample randomly for a better distribution.
		if (total++ >= 3)
			break;

		RkUint32 const other_index {m_avoidance_hash_map.hash_map[other].entity_index};
		Vector3m const offset      {m_positions[other_index] - m_positions[in_boid]};
		Meters   const sqr_dst     {offset.SquareLength()};

		if (sqr_dst > cohesion_distance * cohesion_distance)
			continue;

	    m_avoidance_heading[in_boid] -= offset / std::max(sqr_dst, 0.001_m);
	}
}

AsyncTask<ECSJobQueue> Flock::ApplyCohesionIntent() noexcept
{
	// 1 -- Hashing positions
	m_cohesion_hash_map.cell_size = cohesion_distance;
	co_await ParallelForChunk(0uz, m_boids, 256uz, [&](RkSize const in_index)
	{
		m_cohesion_hash_map.SetHashIndex(m_positions[in_index], in_index);
	});

	{
		ZoneScopedN("Sort Hashes");

		// 2 -- Sorting hashes
		m_cohesion_hash_map.SortHashes();
	}

	// 3 -- Compute cohesion vectors
	co_await ParallelForChunk(0uz, m_boids, 64uz, [&](RkSize const in_index) {

		// Get a reference position
	    Vector3m const position = m_positions[in_index];

	    m_number_flockmates[in_index] = 0;
	    m_flock_heading    [in_index] = Constants<Vector3m>::zero;
		m_flock_center     [in_index] = Constants<Vector3m>::zero;

		for (Vector3m offset : HashGrid::s_3d_neighbors)
		{
			// Check if the cell exists
			RkUint32 const			hash       = m_cohesion_hash_map.MakeHash(offset * cohesion_distance + position);
			std::optional<RkUint32> hash_index = m_cohesion_hash_map.SearchFirst(hash);
			if (!hash_index.has_value())
				continue;

			[[likely]]

			// Accumulate stats for each boid in the current cell
			UpdateCohesionStatsForIndex(in_index, hash, hash_index.value());
		}
	});

	// 4 -- Apply results
	co_await ParallelForChunk(0uz, m_boids, 128uz, [&](RkSize const in_index) {

		Vector3m const flock_center {
			m_number_flockmates[in_index] == 0 ? Constants<Vector3m>::zero
			: (m_flock_center[in_index] / m_number_flockmates[in_index] - m_positions[in_index]).Normalized()
		};

		Vector3m const flock_heading {
			m_number_flockmates[in_index] == 0 ? Constants<Vector3m>::zero
			: m_flock_heading[in_index].Normalized()
		};

		m_accelerations[in_index] += flock_center  * cohesion_weight;
		m_accelerations[in_index] += flock_heading * alignment_weight;
	});
}

AsyncTask<ECSJobQueue> Flock::ApplySeparationIntent() noexcept
{
	// 1 -- Hashing positions
	m_avoidance_hash_map.cell_size = separation_distance;
	co_await ParallelForChunk(0uz, m_boids, 256uz, [&](RkSize const in_index)
	{
		m_avoidance_hash_map.SetHashIndex(m_positions[in_index], in_index);
	});

	{
		ZoneScopedN("Sort Hashes");

		// 2 -- Sorting hashes
		m_avoidance_hash_map.SortHashes();
	}

	// 3 -- Compute separation vectors
	co_await ParallelForChunk(0uz, m_boids, 64uz, [&](RkSize const in_index)
	{
		// Get a reference position
		Vector3m const position = m_positions[in_index];
		m_avoidance_heading[in_index] = Constants<Vector3m>::zero;

		for (Vector3m offset : HashGrid::s_3d_neighbors)
		{
			// Check if the cell exists
			RkUint32 const			hash       = m_avoidance_hash_map.MakeHash(offset * cohesion_distance + position);
			std::optional<RkUint32> hash_index = m_avoidance_hash_map.SearchFirst(hash);
			if (!hash_index.has_value())
				continue;

			[[likely]]

			// Accumulate stats for each boid in the current cell
			UpdateAvoidanceStatsForIndex(in_index, hash, hash_index.value());
		}
	});

	// 4 -- Apply Avoidance Vectors
	co_await ParallelForChunk(0uz, m_boids, 128uz, [&](RkSize const in_index)
	{
		Vector3m const direction {
			m_number_flockmates[in_index] == 0 ? Constants<Vector3m>::zero
			: (m_avoidance_heading[in_index] / m_number_flockmates[in_index]).Normalized()
		};

		m_accelerations[in_index] += direction * avoidance_weight;
	});
}

AsyncTask<ECSJobQueue> Flock::ApplyTargetIntent() noexcept
{
	co_await ParallelForChunk(0uz, m_boids, 1024uz, [&](RkSize const in_index) {
		m_accelerations[in_index] += (target_position - m_positions[in_index]).Normalized() * target_weight;
	});
}

AsyncTask<ECSJobQueue> Flock::ApplyHeightIntent(Seconds const& in_delta_time) noexcept
{
	co_await ParallelForChunk(0uz, m_boids, 128uz, [&](RkSize const in_index)
	{
		RkFloat  const level_distance {Abs(static_cast<RkFloat>(m_positions[in_index].y() - target_height))};
		RkFloat	 const intent_weight  {SmoothStep(0.0f, Abs(static_cast<RkFloat>(target_height_range)), level_distance)};
		Vector3m const next_position  {m_positions[in_index] + m_velocities[in_index] * static_cast<RkFloat>(in_delta_time)};
		Vector3m const target		  {next_position.x(), target_height, next_position.z()};

		m_accelerations[in_index] += (target - m_positions[in_index]).Normalized() * intent_weight * height_weight;
	});
}

AsyncTask<ECSJobQueue> Flock::MoveBoids(Seconds const& in_delta_time)
{
	co_await ParallelForChunk(0uz, m_boids, 128uz, [&](RkSize const in_index)
	{
		m_velocities   [in_index]  = IntegrateVelocity(in_index, in_delta_time);
		m_positions    [in_index] += m_velocities[in_index] * static_cast<RkFloat>(in_delta_time);
		m_accelerations[in_index]  = Constants<Vector3m>::zero;
	});
}

Vector3m Flock::IntegrateVelocity(RkSize const in_index, Seconds in_delta_time) const noexcept
{
	Vector3m const velocity = m_velocities[in_index] + m_accelerations[in_index] * static_cast<RkFloat>(in_delta_time);
	RkFloat		   speed    = static_cast<RkFloat>(velocity.Length());

	if (speed == 0.0f)
		return Constants<Vector3m>::zero;

	Vector3m const dir = velocity / speed;
	speed = Clamp(speed, minimal_boid_speed, maximal_boid_speed);

	// Returning clamped velocity
	return dir * speed;
}

AsyncTask<ECSJobQueue> Flock::ApplyTransform()
{
	co_await ParallelForChunk(0uz, m_boids, 128uz, [&](RkSize const in_index)
	{
		auto const direction {m_velocities[in_index].Normalized()};
		auto const right	 {Constants<Vector3m>::up.Cross(direction)};
		auto const forward   {direction.Cross(right)};

		Quaternion const rotation (-right, ArcCos(forward.Dot(direction)));

		m_render_matrices[in_index] = Matrix4x4::ModelMatrix(
			m_positions[in_index],
			rotation,
			Constants<Vector3m >::one / 3.0f
		);
	});
}