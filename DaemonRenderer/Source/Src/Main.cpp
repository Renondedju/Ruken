/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <iostream>

#include "Config.hpp"

#include "Threading/Scheduler.hpp"
#include "Resource/ResourceManager.hpp"
#include "Resource/ResourceLoadingDescriptor.hpp"

USING_DAEMON_NAMESPACE

struct TestResource final : IResource
{
	DAEvoid Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override
	{
		std::cout << "Loading Resource" << std::endl;
	}

	DAEvoid Reload(ResourceManager& in_manager) override
	{
		std::cout << "Reloading Resource" << std::endl;
	}

	DAEvoid Unload(ResourceManager& in_manager) noexcept override
	{
		std::cout << "Unloading Resource" << std::endl;
	}
};

int main()
{
	Scheduler		scheduler;
	ResourceManager resource_manager(scheduler);

	Handle<TestResource> const resource = resource_manager.RequestResource<TestResource>({"Test resource"}, {}, EResourceLoadingMode::Asynchronous);

	while(!resource.Available())
		std::this_thread::yield();

	system("pause");

	return EXIT_SUCCESS;
}
