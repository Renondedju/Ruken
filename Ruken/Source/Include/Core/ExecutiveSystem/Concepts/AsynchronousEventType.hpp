#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept AsynchronousEventType = requires (TType in_type)
{
    static_cast<AsynchronousEvent<typename TType::ProcessingUnit>>(in_type);
};

END_RUKEN_NAMESPACE