#pragma once

#include "../Modules/ECS/Components/EntityComponent.hpp"
#include "Core/Maths/Matrix/Matrix.hpp"
#include "Core/Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

struct HealthComponent final : EntityComponent<RkFloat>
{};

END_RUKEN_NAMESPACE
