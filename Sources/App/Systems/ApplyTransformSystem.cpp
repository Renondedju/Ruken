#include "ApplyTransformSystem.hpp"

USING_RUKEN_NAMESPACE

ApplyTransformSystem::ApplyTransformSystem(Universe& in_universe) noexcept
{
	SetupEventHandler<>();
}

RkVoid ApplyTransformSystem::Execute(
	TransformComponent::Position const& in_position,
	TransformComponent::Velocity const& in_velocity,
	TransformComponent::TRS&		  out_transform) noexcept
{
	out_transform = Matrix4x4::ModelMatrix(in_position, {}, Constants<Vector3m>::one);
}

