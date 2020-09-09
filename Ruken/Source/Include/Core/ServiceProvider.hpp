/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#include <type_traits>
#include <unordered_map>

#include "Build/Namespace.hpp"
#include "Core/Service.hpp"
#include "Core/ServiceBase.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Locates and provides services
 */
class ServiceProvider
{
    private:

        #pragma region Members

        // Provided services key = service id, value = service pointer
        std::unordered_map<RkSize, ServiceBase*> m_services;

        #pragma endregion

    public:

        #pragma region Constructors

        ServiceProvider()                               = default;
        ServiceProvider(ServiceProvider const& in_copy) = default;
        ServiceProvider(ServiceProvider&&      in_move) = default;
        ~ServiceProvider() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Provides and holds a service to allow others parts of the code to locate it later on if needed
         * \tparam TService Service type, must inherit from the Service class
         * \tparam TArgs TService constructor types, excluding the service provider instance type
         * \param in_args Arguments to pass to the TService constructor, excluding the service provider instance
         * \warning Providing a service that has already been provided
         *          will override the previous instance without destroying it, leading to memory leaks
         * \return New service instance
         */
        template <typename TService, typename... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, RkBool> = true>
        TService* ProvideService(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>);

        /**
         * \brief Forces the destruction of a service prematurely.
         *        This is useful to enforce a given destruction order.
         * \note Any left over services will be cleaned up anyway in the destructor of the provider but the destruction order is not guaranteed 
         * \tparam TService Service type
         */
        template <typename TService>
        RkVoid DestroyService() noexcept;

        /**
         * \brief Locates a service. This service could be unavailable or unprovided yet, if this is the case, a nullptr will be returned
         * \tparam TService Service type, must inherit from the Service class
         * \return Located service, could be null if the service is unavailable or unprovided.
         */
        template <typename TService>
        [[nodiscard]] TService* LocateService() noexcept;

        #pragma endregion

        #pragma region Operators
        
        ServiceProvider& operator=(ServiceProvider const& in_copy) = default;
        ServiceProvider& operator=(ServiceProvider&&      in_move) = default;

        #pragma endregion
};

#include "Core/ServiceProvider.inl"

END_RUKEN_NAMESPACE