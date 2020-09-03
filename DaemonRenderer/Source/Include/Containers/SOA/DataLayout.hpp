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

#include <tuple>
#include <utility>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class describes an SOA layout and implements an interface to interact with the given layout
 * \tparam TContainer Internal container class
 * \tparam TLayoutTypes Types to be contained in the layout
 */
template <template <typename> class TContainer, typename... TLayoutTypes>
class DataLayout
{
    public:

        using ContainerType = std::tuple<TContainer<TLayoutTypes>...>;

    private:

        #pragma region Constructors

        DataLayout()                          = default;
        DataLayout(DataLayout const& in_copy) = default;
        DataLayout(DataLayout&&      in_move) = default;
        ~DataLayout()                         = default;

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Getter helper 
         * \tparam TLayoutView View type to operate with
         * \tparam TIds Index sequence of the view
         * \param in_container Container instance
         * \param in_position position of the get
         * \return View instance containing references to the requested resources
         */
        template <typename TLayoutView, DAEsize... TIds>
        constexpr static auto GetHelper(ContainerType& in_container, DAEsize in_position, std::index_sequence<TIds...>) noexcept;

        /**
         * \brief Resize helper
         * \tparam TIds Index sequence of the layout
         * \param in_container Container instance
         * \param in_size New size
         */
        template <DAEsize... TIds>
        constexpr static DAEvoid ResizeHelper(ContainerType& in_container, DAEsize in_size, std::index_sequence<TIds...>) noexcept;

        /**
         * \brief Push back helper
         * \tparam TValue Values to push back
         * \tparam TIds Index sequence of the layout
         * \param in_container Container instance
         * \param in_value Value instance to push back
         */
        template <typename TValue, DAEsize... TIds>
        constexpr static DAEvoid PushBackHelper(ContainerType& in_container, TValue&& in_value, std::index_sequence<TIds...>) noexcept;

        #pragma endregion

        #pragma region Operators

        DataLayout& operator=(DataLayout const& in_copy) = default;
        DataLayout& operator=(DataLayout&&      in_move) = default;

        #pragma endregion

    public:

        #pragma region Methods

        /**
         * \brief Get method, this method operates with a layout view allowing the fetch only the requested data
         * \tparam TLayoutView Layout view type
         * \param in_container Container instance
         * \param in_position Position to get the data at
         * \return View instance containing references to the requested resources
         */
        template <typename TLayoutView>
        constexpr static auto Get(ContainerType& in_container, DAEsize in_position) noexcept;

        /**
         * \brief Allows to resize the layout underlying container
         * \param in_container Container instance
         * \param in_size New size
         */
        constexpr static DAEvoid Resize(ContainerType& in_container, DAEsize in_size) noexcept;

        /**
         * \brief Pushes back new values into the underlying containers
         * \tparam TValue Value type
         * \param in_container Container instance
         * \param in_value Value instance to push back
         */
        template <typename TValue>
        constexpr static DAEvoid PushBack(ContainerType& in_container, TValue&& in_value) noexcept;

        /**
         * \brief Returns the size of the layout
         * \param in_container Container instance
         * \return Container size
         */
        constexpr static DAEsize Size(ContainerType const& in_container) noexcept;

        #pragma endregion 
};

#include "Containers/SOA/DataLayout.inl"

END_RUKEN_NAMESPACE
