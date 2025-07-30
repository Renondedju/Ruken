#include "ResourceManager.hpp"

USING_RUKEN_NAMESPACE

ResourceManager::ResourceManager(ServiceProvider& in_parent):
	Service {in_parent, typeid(ResourceManager)}
{}
