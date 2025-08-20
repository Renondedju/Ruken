#pragma once

#include "Core/Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

enum class ELogLevel : RkUint8
{
    Debug,     ///@brief Detailed information, typically of interest only when diagnosing problems.
    Info,     ///@brief Confirmation that things are working as expected.
    Warning, ///@brief An indication that something unexpected happened, or indicative of some problem in the near future.
    Error   ///@brief The software has not been able to perform some function.
};

END_RUKEN_NAMESPACE