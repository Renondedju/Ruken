#include "Debug/TraceAllocations.hpp"
#include "Build/BuildInfo.hpp"

#include <tracy/Tracy.hpp>

void* operator new(std::size_t const _Size, RUKEN_NAMESPACE::TracyNoAllocationTracking)
{ return std::malloc(_Size); }

void  operator delete(void* _Block, RUKEN_NAMESPACE::TracyNoAllocationTracking)
{ return std::free(_Block); }

#ifdef TRACY_ENABLE

	void* operator new (std::size_t const _Size)
	{
		ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

		auto const ptr {std::malloc(_Size)};
		TracyAlloc(ptr, _Size);
		return ptr;
	}

	void operator delete(void* _Block) noexcept
	{
		ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

		TracyFree(_Block);
		std::free(_Block);
	}

	void operator delete(void* _Block, std::size_t) noexcept
	{
		ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_ALLOC_ZONES));

		TracyFree(_Block);
		std::free(_Block);
	}

#endif