
#include "Utility/Benchmark.hpp"

#include <iostream>

USING_RUKEN_NAMESPACE

Benchmark::Benchmark(const RkChar* in_label, RkUint64 const in_execution_count) noexcept:
    m_label {in_label},
    m_execution_count {in_execution_count},
    m_time  {std::chrono::steady_clock::now()}
{}

Benchmark::~Benchmark()
{
    if (m_execution_count == 1)
    {
        std::cout << "Benchmark labeled '" << m_label << "' took: "
            << std::chrono::duration<RkDouble, std::milli>(std::chrono::steady_clock::now() - m_time).count()
            << "ms" << std::endl;
    }
    else
    {
        std::cout << "Benchmark labeled '" << m_label << "' took: "
            << std::chrono::duration<RkDouble, std::milli>(std::chrono::steady_clock::now() - m_time).count()
            << "ms for " << m_execution_count << " executions (~ " <<  std::chrono::duration<RkDouble, std::milli>(std::chrono::steady_clock::now() - m_time).count() / m_execution_count
            << "ms/execution)" << std::endl;
    }
}
