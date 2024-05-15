#include "Build/BuildInfo.hpp"
#include "Types/FundamentalTypes.hpp"

#include <tracy/Tracy.hpp>

USING_RUKEN_NAMESPACE

#ifdef RUKEN_TRACE_BUILD

void* operator new(RkSize in_count)
{
    auto const ptr {malloc(in_count)};
    TracyAlloc(ptr, in_count);
    return ptr;
}

void operator delete(void* in_ptr) noexcept
{
    TracyFree(in_ptr);
    free     (in_ptr);
}

#endif