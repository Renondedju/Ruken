#pragma once

#include "../Components.hpp"
#include "ECS/System.hpp"

BEGIN_RUKEN_NAMESPACE

struct ApplyTransformSystem final: System
{
	#pragma region Lifetime

	explicit ApplyTransformSystem(Universe& in_universe) noexcept;
	ApplyTransformSystem		   (ApplyTransformSystem const&) = delete;
	ApplyTransformSystem		   (ApplyTransformSystem&&)      = delete;
	ApplyTransformSystem& operator=(ApplyTransformSystem const&) = delete;
	ApplyTransformSystem& operator=(ApplyTransformSystem&&)      = delete;
	~ApplyTransformSystem() override					         = default;

	#pragma endregion

	/// @brief Transforms the position and velocity of a boid into a TRS matrix for rendering.
	RkVoid Execute(TransformComponent::Position const& in_position,
				   TransformComponent::Velocity const& in_velocity,
				   TransformComponent::TRS&			   out_transform) noexcept;
};


END_RUKEN_NAMESPACE