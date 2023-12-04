
#pragma once

#include <chrono>

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Benchmark class, prints the lifetime of the class on destruction
 */
class Benchmark
{
    private:

        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        #pragma region Members

        RkChar const*  m_label;
        RkUint64       m_execution_count;
        TimePoint       m_time;

        #pragma endregion 

    public:

        #pragma region Constructors

        /**
         * \brief Benchmark constructor
         * \param in_label Label of the benchmark
         * \param in_execution_count Number of times that the benchmarked code will be executed
         */
        Benchmark (RkChar const*   in_label, RkUint64 in_execution_count) noexcept;
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
#define RUKEN_BENCHMARK(in_benchmark_label) if (Benchmark const benchmark = Benchmark(in_benchmark_label, 1))

namespace internal
{
    /**
     * \brief This allows us to create 2 variables inside a for loop
     */
    struct BenchmarkIterator
    {
        RkUint64 const loop_count;
        RkUint64       loop_index;
        Benchmark      benchmark;
    };
}

/**
 * \brief Benchmark macro, this is intended to quickly benchmark a part of your code.
 * \param in_benchmark_label Benchmark's label
 * \param in_benchmark_execution_count Number of times to execute the benchmark
 */
#define RUKEN_LOOPED_BENCHMARK(in_benchmark_label, in_benchmark_execution_count) \
    for (internal::BenchmarkIterator benchmark_iterator = {in_benchmark_execution_count, 0, Benchmark(in_benchmark_label, in_benchmark_execution_count)}; benchmark_iterator.loop_index < benchmark_iterator.loop_count; ++benchmark_iterator.loop_index)

END_RUKEN_NAMESPACE