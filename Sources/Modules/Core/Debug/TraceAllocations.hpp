#pragma once

#include <cstddef>
#include <tracy/Tracy.hpp>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

struct TracyNoAllocationTracking {};

template <typename T>
struct UntrackedAllocator
{
	using value_type = T;

	#pragma region Lifetime

	UntrackedAllocator()									 = default;
	UntrackedAllocator(UntrackedAllocator const&)            = default;
	UntrackedAllocator(UntrackedAllocator&&)	             = default;
	UntrackedAllocator& operator=(UntrackedAllocator const&) = default;
	UntrackedAllocator& operator=(UntrackedAllocator&&)		 = default;
	~UntrackedAllocator()									 = default;

	template <typename U>
	explicit constexpr UntrackedAllocator(const UntrackedAllocator<U>&) noexcept {};

	#pragma endregion

	static T*	allocate  (std::size_t _Size);
	static void deallocate(T* _Block, std::size_t _Size) noexcept;
};

template <typename T, typename U>
bool operator==(const UntrackedAllocator<T>&, const UntrackedAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const UntrackedAllocator<T>&, const UntrackedAllocator<U>&) { return false; }


END_RUKEN_NAMESPACE

void* operator new   (std::size_t _Size, RUKEN_NAMESPACE::TracyNoAllocationTracking);
void  operator delete(void*		 _Block, RUKEN_NAMESPACE::TracyNoAllocationTracking);

#ifdef TRACY_ENABLE

	void* operator new (std::size_t _Size);

	void operator delete(void* _Block)				noexcept;
	void operator delete(void* _Block, std::size_t) noexcept;

#endif

BEGIN_RUKEN_NAMESPACE

template<typename T>
T* UntrackedAllocator<T>::allocate(std::size_t const _Size)
{
	return static_cast<T*>(::operator new(_Size * sizeof(T), TracyNoAllocationTracking {}));
}

template<typename T>
void UntrackedAllocator<T>::deallocate(T* _Block, std::size_t) noexcept
{
	::operator delete(_Block, TracyNoAllocationTracking {});
}

END_RUKEN_NAMESPACE
