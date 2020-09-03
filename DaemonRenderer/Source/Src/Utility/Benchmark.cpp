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

#include "Utility/Benchmark.hpp"

#include <iostream>

USING_RUKEN_NAMESPACE

Benchmark::Benchmark(const DAEchar* in_label, DAEuint64 const in_execution_count) noexcept:
    m_label {in_label},
    m_execution_count {in_execution_count},
    m_time  {std::chrono::steady_clock::now()}
{}

Benchmark::~Benchmark()
{
    if (m_execution_count == 1)
    {
        std::cout << "Benchmark labeled '" << m_label << "' took: "
            << std::chrono::duration<DAEdouble, std::milli>(std::chrono::steady_clock::now() - m_time).count()
            << "ms" << std::endl;
    }
    else
    {
        std::cout << "Benchmark labeled '" << m_label << "' took: "
            << std::chrono::duration<DAEdouble, std::milli>(std::chrono::steady_clock::now() - m_time).count()
            << "ms for " << m_execution_count << " executions (~ " <<  std::chrono::duration<DAEdouble, std::milli>(std::chrono::steady_clock::now() - m_time).count() / m_execution_count
            << "ms/execution)" << std::endl;
    }
}
