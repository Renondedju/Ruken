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
#include "Debug/Logging/LogRecord.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class configures the final order, structure, and contents of the log message.
 */
class LogFormatter
{
    protected:

        #pragma region Constructor

        LogFormatter() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_record The record to compute a label for.
         * \return The resulting label.
         */
        [[nodiscard]]
        virtual std::string ComputeLabel(LogRecord const& in_record) const noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        LogFormatter(LogFormatter const&    in_copy) = delete;
        LogFormatter(LogFormatter&&         in_move) = delete;
        virtual ~LogFormatter()                      = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_record The record to format.
         * \return The resulting string.
         */
        [[nodiscard]]
        virtual std::string Format(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         * \return The resulting string.
         */
        [[nodiscard]]
        virtual std::string FormatTime(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         * \return The resulting string.
         */
        [[nodiscard]]
        virtual std::string FormatException(LogRecord const& in_record) const noexcept;

        /**
         * \param in_record The record to format.
         * \return The resulting string.
         */
        [[nodiscard]]
        virtual std::string FormatStack(LogRecord const& in_record) const noexcept;

        #pragma endregion

        #pragma region Operators

        LogFormatter& operator=(LogFormatter const& in_copy) = delete;
        LogFormatter& operator=(LogFormatter&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE