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

#pragma once

#include <memory>
#include <vector>

#include "Vulkan/Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Logger;
class PhysicalDevice;

/**
 * \brief There is no global state in Vulkan and all per-application state is stored in a VkInstance object.
 *        Creating a VkInstance object initializes the Vulkan library and allows the application to pass information about itself to the implementation.
 */
class Instance
{
    private:

        #pragma region Members

        Logger* m_logger;

        VkInstance m_handle;

        std::vector<PhysicalDevice> m_physical_devices;

        #pragma endregion

        #pragma region Constructor


        #pragma endregion
        
        #pragma region Methods

        /**
         * \return True if the API version is equal or higher than the required one, else False.
         */
        static DAEbool CheckInstanceVersion() noexcept;

        /**
         * \return True if the required extensions are supported, else False.
         */
        static DAEbool CheckInstanceExtensions() noexcept;

        /**
         * \return True if the required validation layers are supported, else False.
         */
        static DAEbool CheckValidationLayers() noexcept;

        /**
         * \return True if a new Vulkan instance could be created, else False.
         */
        DAEbool SetupInstance() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Instance() noexcept;

        Instance(Instance const&    in_copy) = delete;
        Instance(Instance&&         in_move) = delete;

        ~Instance() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        static DAEbool Create(std::unique_ptr<Instance>& out_instance);

        /**
         * \return The opaque handle to the instance object.
         */
        [[nodiscard]]
        VkInstance const& GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        std::vector<PhysicalDevice> const& GetPhysicalDevices() const noexcept;

        #pragma endregion

        #pragma region Operators

        Instance& operator=(Instance const& in_copy) = delete;
        Instance& operator=(Instance&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE