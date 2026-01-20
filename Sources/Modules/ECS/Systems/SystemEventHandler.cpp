#include "SystemEventHandler.hpp"

USING_RUKEN_NAMESPACE

SystemEventHandler::SystemEventHandler(ComponentQuery const& in_query) noexcept:
	component_query {in_query}
{}
