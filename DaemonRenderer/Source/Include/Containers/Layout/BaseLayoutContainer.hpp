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
#include "Containers/Layout/EDataLayout.hpp"
#include "Containers/Layout/LayoutIterator.hpp"
#include "Containers/Layout/DataLayoutPolicy.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Base layout container class. This class allows for easy creation of SoA structures, see example below 
 * \tparam TContainer Container type
 * \tparam TDataLayout Data layout type
 * \tparam TItem Container item type
 *
 * \example
 *
 * template<typename... T>
 * class LifeComponentItemBase : Tuple<T...>
 * {
 *     enum EMemberNames : DAEsize
 *     {
 *         Life,
 *         MaxLife
 *     };
 * 
 *     public:
 * 
 *         using Tuple<T...>::tuple;
 * 
 *         auto& GetLife   () { return std::get<EMemberNames::Life>   (*this); }
 *         auto& GetMaxLife() { return std::get<EMemberNames::MaxLife>(*this); }
 * };
 * 
 * using LifeComponentItem    = LifeComponentItemBase<DAEfloat, DAEfloat>;
 * using LifeComponentStorage = BaseLayoutContainer<Vector, EDataLayout::ArrayOfStructures, LifeComponentItem>;
 *
 */
template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
class BaseLayoutContainer
{
    public:

        #pragma region Typedefs

        using Policy    = DataLayoutPolicy<TContainer, TDataLayout, TItem>;
        using Iterator  = LayoutIterator<class BaseLayoutContainer<TContainer, TDataLayout, TItem>>;
	    using ValueType	= typename Policy::ValueType;

	    static EDataLayout constexpr data_layout = TDataLayout;

        #pragma endregion

    private:

        #pragma region Members

	    typename Policy::ContainerType m_values;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_size Base size of the container
         */
        BaseLayoutContainer(DAEsize in_size);
        BaseLayoutContainer(BaseLayoutContainer const& in_copy) noexcept = default;
        BaseLayoutContainer(BaseLayoutContainer&&      in_move) noexcept = default;
        ~BaseLayoutContainer()                                  noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
	     * \brief Pushes back a new value into the container
	     * \tparam TFwd Value type
	     * \param in_val Value to push back
	     */
	    template <typename TFwd>
        DAEvoid PushBack(TFwd&& in_val);

        /**
         * \brief Returns the size of the container
         * \return Container size
         */
        DAEsize Size();

        /**
         * \brief Resizes the container 
         * \param in_size New size
         */
        DAEvoid Resize(DAEsize in_size);

        /**
         * \brief Returns the begin iterator
         * \return Begin iterator instance
         */
        Iterator begin();

        /**
         * \brief Returns the end iterator
         * \return End iterator instance
         */
        Iterator end();

        #pragma endregion 

        #pragma region Operators

        /**
         * \brief Random access operator
         * \param in_position Position
         * \return Value at this position 
         */
        ValueType operator[](DAEsize in_position);

        BaseLayoutContainer& operator=(BaseLayoutContainer const& in_copy) noexcept = default;
        BaseLayoutContainer& operator=(BaseLayoutContainer&&      in_move) noexcept = default;

        #pragma endregion 
};

#include "Containers/Layout/BaseLayoutContainer.inl"

END_DAEMON_NAMESPACE