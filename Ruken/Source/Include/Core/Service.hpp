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

#include "Build/Namespace.hpp"
#include "Core/ServiceBase.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class ServiceProvider;

/**
 * \brief Service base class. Must be inherited from to implement a service.
 * \tparam TCrtp This class uses the Curiously Recurring Template Pattern. This type must be the actual service implementation class.
 */
template <typename TCrtp>
class Service : public ServiceBase
{
    protected:

        #pragma region Members

        ServiceProvider& m_service_provider;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_service_provider Service provider instance
         * \warning Any service service inheriting from this class must declare a default constructor that
         *          takes in the service provider instance as the FIRST parameter.
         */
        Service(ServiceProvider& in_service_provider) noexcept;

        Service(Service const& in_copy) = default;
        Service(Service&&      in_move) = default;
        virtual ~Service()              = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Retrieves the unique ID of the service
         * \return Service ID
         */
        static RkSize ServiceID() noexcept;

        #pragma endregion

        #pragma region Operators
        
        Service& operator=(Service const& in_copy) = default;
        Service& operator=(Service&&      in_move) = default;

        #pragma endregion
};

#include "Core/Service.inl"

END_RUKEN_NAMESPACE