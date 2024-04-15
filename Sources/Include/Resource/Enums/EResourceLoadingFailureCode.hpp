
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Describes the general type of the resource loading error
 * 
 * Other                    => Other failure type than listed bellow. 
 * Unknown                    => Unknown failure.
 * OutOfMemory                => The device does not have enough memory. Retries can be considered. 
 *                               Sending this code will cause the garbage collector to immediately try to free up some memory.
 * NoSuchResource            => The resource has not been found, this might be caused by an invalid path
 * CorruptedResource        => The resource is corrupted or invalid. The resource had unexpected data patterns.
 * UnauthorizedAccess        => The resource exists, but we don't have the rights to access it. Retries can be considered.
 * TemporarilyUnavailable    => The resource is not available for now but might be later, retries can be considered.
 * RequirementsNotSatisfied    => Any of the requirements of the resource isn't satisfied. Retries can be considered.
 */
enum class EResourceProcessingFailureCode : RkUint8
{
    Other,
    Unknown,
    OutOfMemory,
    NoSuchResource,
    CorruptedResource,
    UnauthorizedAccess,
    TemporarilyUnavailable,
    RequirementsNotSatisfied
};

[[nodiscard]]
inline std::string EnumToString(EResourceProcessingFailureCode const in_code)
{
    switch (in_code)
    {
        case EResourceProcessingFailureCode::Other:
            return "Other";
        case EResourceProcessingFailureCode::Unknown:
            return "Unknown";
        case EResourceProcessingFailureCode::NoSuchResource:
            return "NoSuchResource";
        case EResourceProcessingFailureCode::CorruptedResource:
            return "CorruptedResource";
        case EResourceProcessingFailureCode::UnauthorizedAccess:
            return "UnauthorizedAccess";
        case EResourceProcessingFailureCode::TemporarilyUnavailable:
            return "TemporarilyUnavailable";
        case EResourceProcessingFailureCode::RequirementsNotSatisfied:
            return "RequirementsNotSatisfied";
        default:
            return "unknown";
    }
}

END_RUKEN_NAMESPACE