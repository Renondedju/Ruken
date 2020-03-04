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

#include "LogFormatter.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class configures the final order, structure, and contents of the log message for the console.
 */
class ConsoleFormatter final : public LogFormatter
{
    protected:

        #pragma region Members

        DAEvoid* m_handle;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_record The record to compute a label for.
         *
         * \return The resulting label.
         */
        [[nodiscard]] String ComputeLabel(LogRecord const& in_record) const noexcept override;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        ConsoleFormatter() noexcept;

        ConsoleFormatter(ConsoleFormatter const&    in_copy) = delete;
        ConsoleFormatter(ConsoleFormatter&&         in_move) = delete;

        ~ConsoleFormatter() noexcept = default;

        #pragma endregion

        #pragma region Operators

        ConsoleFormatter& operator=(ConsoleFormatter const& in_copy) = delete;
        ConsoleFormatter& operator=(ConsoleFormatter&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        

        #pragma endregion
};

END_DAEMON_NAMESPACE