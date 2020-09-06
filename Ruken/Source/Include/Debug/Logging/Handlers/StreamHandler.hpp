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

#include <ostream>

#include "Debug/Logging/Handlers/LogHandler.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class sends logging output to a stream.
 */
class StreamHandler : public LogHandler
{
    protected:

        #pragma region Members

        std::ostream m_stream;

        #pragma endregion

    public:

        #pragma region Contructors

        explicit StreamHandler(LogFormatter const& in_formatter, std::ostream const& in_stream) noexcept;

        StreamHandler(StreamHandler const& in_copy) = delete;
        StreamHandler(StreamHandler&&      in_move) = delete;

        ~StreamHandler() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Empties the entire queue to the stream.
         * \note  This method must not be called on multiple threads at a time.
         */
        RkVoid Flush() override;

        #pragma endregion

        #pragma region Operators

        StreamHandler& operator=(StreamHandler const& in_other) = delete;
        StreamHandler& operator=(StreamHandler&&      in_other) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE