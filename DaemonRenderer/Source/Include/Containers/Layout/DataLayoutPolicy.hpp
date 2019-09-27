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

#include "Meta/ReferenceWrapper.hpp"
#include "Types/FundamentalTypes.hpp"

#include "Containers/Tuple.hpp"
#include "Containers/Layout/EDataLayout.hpp"
#include "Containers/Layout/LayoutIterator.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Data layout policy class
 * \tparam TContainer Container class
 * \tparam TDataLayout EDataLayout value
 * \tparam TItem Storage item class
 */
template <template <typename...> class TContainer, EDataLayout TDataLayout, typename TItem>
class DataLayoutPolicy;

/**
 * \brief Data layout policy ArrayOfStructures specialization
 * \tparam TContainer Container class
 * \tparam TItem Storage item class
 */
template <template <typename...> class TContainer, typename TItem>
class DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>
{
    public:

        using ContainerType = TContainer<TItem>;
        using ValueType     = TItem&;

        #pragma region Static Methods

        /**
         * \brief Get container interface
         * \param in_container Container instance
         * \param in_position Position to get from
         * \return Requested value
         */
        constexpr static ValueType Get(ContainerType& in_container, DAEsize in_position);

        /**
         * \brief Resize container interface
         * \param in_container Container instance
         * \param in_size New requested size
         */
        constexpr static DAEvoid Resize(ContainerType& in_container, DAEsize in_size);

        /**
         * \brief PushBack container interface
         * \tparam TValue in_value type
         * \param in_container Container instance
         * \param in_value Value to push back
         */
        template <typename TValue>
        constexpr static DAEvoid PushBack(ContainerType& in_container, TValue&& in_value);

        /**
         * \brief Size container interface
         * \param in_container Container instance
         * \return Size of the container
         */
        constexpr static DAEsize Size(ContainerType& in_container);

        #pragma endregion 
};

template <template <typename...> class TContainer, typename... TItem>
class DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>
{
    public:

	    using ContainerType = Tuple<TContainer<TItem>...>;
	    using ValueType     = TItem<ReferenceWrapper<TItem>...>;

        #pragma region Static Methods

        /**
         * \brief Get container interface
         * \param in_container Container instance
         * \param in_position Position
         * \return Requested value
         */
        constexpr static ValueType Get(ContainerType& in_container, DAEsize in_position);

        /**
         * \brief Resize container interface
         * \param in_container Container instance
         * \param in_size Requested new size
         */
        constexpr static DAEvoid Resize(ContainerType& in_container, DAEsize in_size);

        /**
	     * \brief PushBack container interface
	     * \tparam TValue Value type
	     * \param in_container Container instance
	     * \param in_value Value to push back
	     */
	    template <typename TValue>
        constexpr static DAEvoid PushBack(ContainerType& in_container, TValue&& in_value);

        /**
         * \brief Size container interface
         * \param in_container Container instance
         * \return Container size
         */
        static constexpr DAEsize Size(ContainerType& in_container);

        #pragma endregion 

	private:

        #pragma region Static Methods

        /**
	     * \brief Get interface helper
	     * \tparam TIds ID types
	     * \param in_container Container instance
	     * \param in_position Position
	     * \return Value
	     */
	    template <unsigned... TIds>
        constexpr static auto GetHelper(ContainerType&                         in_container,
                                        DAEsize                                in_position,
                                        std::integer_sequence<DAEsize, TIds...>);

        /**
	     * \brief Resize interface helper 
	     * \tparam TIds ID types
	     * \param in_container Container instance
	     * \param in_size Requested new size
	     */
	    template <unsigned... TIds>
        constexpr static DAEvoid ResizeHelper(ContainerType&                         in_container,
                                              DAEsize                                in_size,
                                              std::integer_sequence<DAEsize, TIds...>);

        /**
	     * \brief PushBack interface helper
	     * \tparam TValue Value type
	     * \tparam TIds ID types
	     * \param in_container Container instance
	     * \param in_value Value to push back
	     */
	    template <typename TValue, DAEsize... TIds>
        constexpr static DAEvoid PushBackHelper(ContainerType&                         in_container,
                                                TValue&&                               in_value,
                                                std::integer_sequence<DAEsize, TIds...>);

        #pragma endregion 
};

#include "Containers/Layout/DataLayoutPolicy.inl"

END_DAEMON_NAMESPACE