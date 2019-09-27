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

#include <limits>
#include <iterator>

#include "Config.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Layout iterator. This class is meant to be used in conjunction with layout containers
 * \tparam TContainer Container type
 */
template <typename TContainer>
class LayoutIterator
{
    private:

        #pragma region Memebers

	    TContainer* m_container = nullptr;
	    DAEsize     m_position  = std::numeric_limits<DAEsize>::infinity();

		#pragma endregion 

    public:

        #pragma region Iterator Traits

	    using Policy = typename TContainer::Policy;

        // Iterator traits
	    using value_type	    = typename Policy::ValueType;
	    using difference_type   = std::ptrdiff_t;
	    using reference			= value_type&;
	    using iterator_category = std::bidirectional_iterator_tag;

        #pragma endregion 

        #pragma region Constructors

        /**
	     * \brief Default constructor
	     * \tparam TTContainer Container type
	     * \param in_container Container instance
	     * \param in_position Position to start at (default is 0)
	     */
	    template<typename TTContainer>
        LayoutIterator(TTContainer* in_container, DAEsize in_position = 0);

        LayoutIterator(LayoutIterator const& in_copy) noexcept = default;
        LayoutIterator(LayoutIterator&&      in_move) noexcept = default;
        ~LayoutIterator()                             noexcept = default;

        #pragma endregion 

        #pragma region Operators

        /**
         * \brief Assignment operators
         * \param in_copy other instance
         * \return This instance
         */
        LayoutIterator& operator=(LayoutIterator const& in_copy) noexcept;
        LayoutIterator& operator=(LayoutIterator&&      in_move) noexcept = default;
        LayoutIterator& operator=(std::nullptr_t const&);

        /**
         * \brief Comparison operators
         * \param in_other Other instance
         * \return Comparison result
         */
        DAEbool operator!=(LayoutIterator const& in_other);
        DAEbool operator==(LayoutIterator const& in_other);

        /**
         * \brief Checks if the iterator is valid
         */
        explicit operator DAEbool() const;

        /**
	     * \brief Increment operator
	     * \tparam T Incrementation type
	     * \param in_size Incrementation value
	     */
	    template <typename T>
        void operator+=(T in_size);
        void operator++();

        /**
         * \brief Decrement operator
         * \tparam T Decremental type
         * \param in_size Decremental value
         */
        template <typename T>
        void operator-=(T in_size);
        void operator--();

        /**
         * \brief Dereferencing operator
         * \return Value of the iterator
         */
        value_type operator*();

        #pragma endregion 
};

#include "Containers/Layout/LayoutIterator.inl"

END_DAEMON_NAMESPACE
