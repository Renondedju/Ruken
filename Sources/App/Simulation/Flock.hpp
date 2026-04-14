#pragma once

#include "ECS/ECSTask.hpp"
#include "Core/Maths/Matrix/Matrix.hpp"
#include "Core/Maths/Vector/DistanceVector.hpp"
#include "Core/Types/Units/Duration/Duration.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"

#include <vector>

#include "HashGrid.hpp"

BEGIN_RUKEN_NAMESPACE

// A flock of boids
struct Flock
{
	#pragma region Members

	// Params
	RkFloat alignment_weight    {40.0f};
	RkFloat cohesion_weight     {15.0f};
	RkFloat avoidance_weight    {75.0f};
	RkFloat height_weight       {5.0f};
	RkFloat target_weight       {2.5f};
	Meters  target_height		{0.0f};
	Meters  target_height_range {1.0f};
	RkFloat minimal_boid_speed  {7.0f};
	RkFloat maximal_boid_speed  {10.0f};
	Meters  cohesion_distance   {150_cm};
	Meters  separation_distance {50_cm};
	Vector3m target_position    {0_m, 0_m, 0_m};

	#pragma endregion

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_size Amount of boids to summon
	 */
	explicit Flock(RkSize in_size);
	Flock(Flock const&) 		   = delete;
	Flock(Flock&&)      		   = delete;
	Flock& operator=(Flock const&) = delete;
	Flock& operator=(Flock&&)      = delete;
	~Flock()					   = default;

	#pragma endregion

	AsyncTask<ECSJobQueue> Update(ServiceProvider const& in_service_provider) noexcept;

	std::vector<Matrix4x4> const& GetTransforms() const noexcept { return m_render_matrices; }

private:

		#pragma region Members

		// Permanent
		RkSize				  m_boids;
		std::vector<Vector3m> m_positions;
		std::vector<Vector3m> m_velocities;
		std::vector<Vector3m> m_accelerations;

		// Transient data
		HashGrid 			   m_cohesion_hash_map;
		HashGrid 			   m_avoidance_hash_map;
		std::vector<Vector3m>  m_flock_heading;
		std::vector<Vector3m>  m_avoidance_heading;
		std::vector<Vector3m>  m_flock_center;
		std::vector<RkUint16>  m_number_flockmates;
		std::vector<Matrix4x4> m_render_matrices;

		#pragma endregion

		#pragma region Methods

		// Intents & Jobs
		AsyncTask<ECSJobQueue> ApplyCohesionIntent  () noexcept;
		AsyncTask<ECSJobQueue> ApplySeparationIntent() noexcept;
		AsyncTask<ECSJobQueue> ApplyTargetIntent    () noexcept;
		AsyncTask<ECSJobQueue> ApplyHeightIntent    (Seconds const& in_delta_time) noexcept;
 		AsyncTask<ECSJobQueue> MoveBoids			(Seconds const& in_delta_time);
		AsyncTask<ECSJobQueue> ApplyTransform();

		Vector3m IntegrateVelocity(RkSize in_index, Seconds in_delta_time) const noexcept;

		RkVoid UpdateCohesionStatsForIndex (RkSize in_boid, RkUint32 in_hash, RkUint32 in_hash_index);
		RkVoid UpdateAvoidanceStatsForIndex(RkSize in_boid, RkUint32 in_hash, RkUint32 in_hash_index);

		#pragma endregion
};

END_RUKEN_NAMESPACE