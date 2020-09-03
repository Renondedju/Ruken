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

#include <chrono>

#include "Config.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Benchmark class, prints the lifetime of the class on destruction
 */
class Benchmark
{
    private:

        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

        #pragma region Members

        DAEchar const*  m_label;
        DAEuint64       m_execution_count;
        TimePoint       m_time;

        #pragma endregion 

    public:

        #pragma region Constructors

        /**
         * \brief Benchmark constructor
         * \param in_label Label of the benchmark
         * \param in_execution_count Number of times that the benchmarked code will be executed
         */
        Benchmark (DAEchar const*   in_label, DAEuint64 in_execution_count) noexcept;
        Benchmark (Benchmark const& in_copy)          = default;
        Benchmark (Benchmark&&      in_move) noexcept = default;
        ~Benchmark();

        #pragma endregion

        #pragma region Operators

        // This is a little trick for the macro below
        explicit constexpr operator bool() const { return true; }
        
        Benchmark& operator=(Benchmark const& in_copy)          = default;
        Benchmark& operator=(Benchmark&&      in_move) noexcept = default;

        #pragma endregion
};

/**
 * \brief Benchmark macro, this is intended to quickly benchmark a part of your code.
 * \param in_benchmark_label Benchmark's label
 */
#define BENCHMARK(in_benchmark_label) if (Benchmark const benchmark = Benchmark(in_benchmark_label, 1))

namespace internal
{
    /**
     * \brief This allows us to create 2 variables inside a for loop
     */
    struct BenchmarkIterator
    {
        DAEuint64 loop_count;
        Benchmark benchmark;
    };
}

/**
 * \brief Benchmark macro, this is intended to quickly benchmark a part of your code.
 * \param in_benchmark_label Benchmark's label
 * \param in_benchmark_exection_count Number of times to execute the benchmark
 */
#define LOOPED_BENCHMARK(in_benchmark_label, in_benchmark_exection_count) \
    for (internal::BenchmarkIterator benchmark_iterator = {0, Benchmark(in_benchmark_label, in_benchmark_exection_count)}; benchmark_iterator.loop_count < in_benchmark_exection_count; ++benchmark_iterator.loop_count)

END_RUKEN_NAMESPACE