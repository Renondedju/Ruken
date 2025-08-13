#include "ResourceManager.hpp"

USING_RUKEN_NAMESPACE

ResourceManager::ResourceManager(ServiceProvider& in_parent):
	Service {in_parent, typeid(ResourceManager)}
{}
/*
IOTask<RkVoid> ResourceManager::Import(FilesystemPath const& in_file_path)
{



}
*/