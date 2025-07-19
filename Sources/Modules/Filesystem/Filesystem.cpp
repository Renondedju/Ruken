#include "Filesystem/Filesystem.hpp"

USING_RUKEN_NAMESPACE

Filesystem::Filesystem(ServiceProvider& in_service_provider) noexcept:
	Service {in_service_provider, typeid(Filesystem)}
{}