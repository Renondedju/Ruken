#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Queue pipeline policy.
 * Ordered from highest to lowest priority:
 *
 * - Blocking: The queue will block a pipeline change until empty (unless the next pipeline also uses that queue).
 * This policy should be used for frame critical work such as gameplay updates.
 *
 * - Delayable: Work can be delayed for multiple frames but is still required for gameplay (ie. path-finding).
 * Delayable work may still be ran after a pipeline change if the hardware has the capacity for it.
 * TODO: Make a function to block pipeline change until a given event has been ran to completion.
 *
 * - Interruptable: Work can be interrupted all-together and will only be resumed when a pipeline requires it.
 * This policy is useful for long editor work that could impact play-mode performances.
 */
enum class ECPUPipelinePolicy
{
	Blocking,
    Delayable,
    Interruptable
};

END_RUKEN_NAMESPACE