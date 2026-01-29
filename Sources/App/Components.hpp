#pragma once

#include "../Modules/ECS/Components/EntityComponent.hpp"
#include "Core/Maths/Matrix/Matrix.hpp"
#include "Core/Maths/Vector/DistanceVector.hpp"

BEGIN_RUKEN_NAMESPACE

struct Position  final: EntityComponent<Vector3m>   { using EntityComponent::EntityComponent; };
struct Rotation  final: EntityComponent<Quaternion> { using EntityComponent::EntityComponent; };
struct Scale     final: EntityComponent<Vector3m>   { using EntityComponent::EntityComponent; };
struct Transform final: EntityComponent<Matrix4x4>  { using EntityComponent::EntityComponent; };

struct HealthComponent final : EntityComponent<RkFloat>
{ using EntityComponent::EntityComponent; };

END_RUKEN_NAMESPACE
