#include "ExecutiveSystem/CPU/Awaitables/Primitives/AutomaticResetEvent.hpp"

USING_RUKEN_NAMESPACE

RkBool AutomaticResetEvent::Signal() const noexcept
{
	return SignalIf([](CPUAwaiter* ){ return true; });
}
