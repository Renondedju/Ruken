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

#include "Config.hpp"

#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"

#include "Containers/SmartPtr.hpp"

#include "Debug/Logging/Logger.hpp"

BEGIN_DAEMON_NAMESPACE

class Renderer
{
    private:

        #pragma region Members

        Logger* m_logger;

        UniquePtr<Instance>         m_instance;
        UniquePtr<PhysicalDevice>   m_physical_device;
        UniquePtr<LogicalDevice>    m_logical_device;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Renderer();

        Renderer(Renderer const&    in_copy) = delete;
        Renderer(Renderer&&         in_move) = delete;

        ~Renderer() noexcept;

        #pragma endregion

        #pragma region Operators

        Renderer& operator=(Renderer const& in_copy) = delete;
        Renderer& operator=(Renderer&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \return This module's logger.
         */
        [[nodiscard]] Logger* GetLogger() const noexcept;

        #pragma endregion
};

/* TODO Needs to be removed when Kernel is done TODO */

extern Renderer* GRenderer;

/* TODO Needs to be removed when Kernel is done TODO */

END_DAEMON_NAMESPACE