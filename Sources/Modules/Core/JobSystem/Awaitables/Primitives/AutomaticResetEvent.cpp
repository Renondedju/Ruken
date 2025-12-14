#include "JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp"

USING_RUKEN_NAMESPACE

RkBool AutomaticResetEvent::Signal() const noexcept
{
	return SignalIf([](AsyncAwaiter* ){ return true; });
}
