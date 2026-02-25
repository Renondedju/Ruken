#pragma once

#include "../Components.hpp"
#include "ECS/Systems/VariadicEventHandler.hpp"

BEGIN_RUKEN_NAMESPACE

struct ApplyTransform final: System
{
	explicit ApplyTransform(Universe& in_universe) noexcept:
		System {in_universe}
	{
		SetupEventHandler<ApplyTransformHandler>({
			EECSEventName::OnStart,
			EECSEventName::OnUpdate,
			EECSEventName::OnEnd,
		});
	}

	struct ApplyTransformHandler final: VariadicEventHandler<const Position, const Rotation, const Scale, Transform>
	{
		using VariadicEventHandler::VariadicEventHandler;
		using VariadicEventHandler::operator=;

		/// @brief Transforms the position and velocity of a boid into a TRS matrix for rendering.
		RkVoid Execute(RkSize in_chunk_index, RkSize const in_chunk_size, ComponentAccess& in_ctx) const noexcept override
		{
			ZoneScoped;

			for (RkSize index {0}; index < in_chunk_size; index++)
			{
				Fetch<Transform>(in_ctx, index) = Matrix4x4::ModelMatrix(
					Fetch<Position>(in_ctx, index),
					Fetch<Rotation>(in_ctx, index),
					Fetch<Scale	  >(in_ctx, index)
				);
			}
		}
	};
};

struct ApplyTransform2 final: System
{
	explicit ApplyTransform2(Universe& in_universe) noexcept:
		System {in_universe}
	{
		SetupEventHandler<ApplyTransformHandler2>({
			EECSEventName::OnStart,
			EECSEventName::OnUpdate,
			EECSEventName::OnEnd,
		});
	}

	struct ApplyTransformHandler2 final: VariadicEventHandler<const Position, Transform>
	{
		using VariadicEventHandler::VariadicEventHandler;
		using VariadicEventHandler::operator=;

		/// @brief Transforms the position and velocity of a boid into a TRS matrix for rendering.
		RkVoid Execute(RkSize in_chunk_index, RkSize const in_chunk_size, ComponentAccess& in_ctx) const noexcept override
		{
			ZoneScoped;

			for (RkSize index {0}; index < in_chunk_size; index++)
			{
				Fetch<Transform>(in_ctx, index) = Matrix4x4::ModelMatrix(
					Fetch<Position>(in_ctx, index), {}, {}
				);
			}
		}
	};
};

struct MoveSystem final: System
{
	explicit MoveSystem(Universe& in_universe) noexcept:
		System {in_universe}
	{
		SetupEventHandler<ApplyMoveHandler>({
			EECSEventName::OnUpdate
		});
	}

	struct ApplyMoveHandler final: VariadicEventHandler<Position>
	{
		using VariadicEventHandler::VariadicEventHandler;
		using VariadicEventHandler::operator=;

		/// @brief Transforms the position and velocity of a boid into a TRS matrix for rendering.
		RkVoid Execute(RkSize in_chunk_index, RkSize const in_chunk_size, ComponentAccess& in_ctx) const noexcept override
		{
			ZoneScoped;

			for (RkSize index {0}; index < in_chunk_size; index++)
				Fetch<Position>(in_ctx, index).Normalize();
		}
	};
};

END_RUKEN_NAMESPACE