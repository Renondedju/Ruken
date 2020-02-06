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

#include <ostream>

#include "LogHandler.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class, located in the core logging package, sends logging output to streams.
 */
class StreamHandler : LogHandler
{
    protected:

        #pragma region Variables



        #pragma endregion

    public:

        #pragma region Contructors and Destructor

        StreamHandler(ELogLevel in_level = ELogLevel::NotSet) noexcept;

		StreamHandler(StreamHandler const&  in_copy) noexcept = default;
		StreamHandler(StreamHandler&&       in_move) noexcept = default;

		~StreamHandler() noexcept = default;

        #pragma endregion

        #pragma region Operators

        StreamHandler& operator=(StreamHandler const& in_other) noexcept = default;
        StreamHandler& operator=(StreamHandler&&      in_other) noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief If a formatter is specified, it is used to format the record. The record is then written to the stream with a terminator.
         *
         * \note If exception information is present, it is formatted and appended to the stream.
         */
        DAEvoid Emit(LogRecord const& in_record) override;

        /**
         * \brief Flushes the stream by calling its Flush() method.
         *
         * \note The Close() method is inherited from "Handler" and so does no output, so an explicit Flush() call may be needed at times.
         */
        DAEvoid Flush() override;

        /**
         * \brief Sets the instanceÅfs stream to the specified value, if it is different.
         *
         * \note The old stream is flushed before the new stream is set.
         */
        DAEvoid SetStream(std::ostream&& in_stream) noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE