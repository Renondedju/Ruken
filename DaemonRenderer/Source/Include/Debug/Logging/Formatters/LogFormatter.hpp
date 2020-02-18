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

#include "../LogRecord.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Formatter objects configure the final order, structure, and contents of the log message.
 */
class LogFormatter
{
    protected:

        #pragma region

        virtual String ComputeLabel(LogRecord const& in_record) const noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        LogFormatter() = default;

        LogFormatter(LogFormatter const&    in_copy) noexcept = default;
        LogFormatter(LogFormatter&&         in_move) noexcept = default;

        virtual ~LogFormatter() = default;

        #pragma endregion

        #pragma region Operators

        LogFormatter& operator=(LogFormatter const& in_copy) noexcept = default;
        LogFormatter& operator=(LogFormatter&&      in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_record The record to format.
         *
         * \return The resulting string.
         */
        virtual String Format(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         *
         * \return The resulting string.
         */
        virtual String FormatTime(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         *
         * \return The resulting string.
         */
        virtual String FormatException(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         *
         * \return The resulting string.
         */
        virtual String FormatStack(LogRecord const& in_record) const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE