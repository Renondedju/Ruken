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

#include "Core/ServiceBase.hpp"

USING_RUKEN_NAMESPACE

RkSize ServiceBase::GetNextId() noexcept
{
    static RkSize id = 0;

    return id++;
}

RkVoid ServiceBase::SignalServiceInitializationFailure(std::string&& in_reason) noexcept
{
    m_status         = EInitializationStatus::Failed;
    m_failure_reason = std::forward<std::string>(in_reason);
}

EInitializationStatus ServiceBase::CheckInitializationStatus(std::string_view& out_reason) const noexcept
{
    out_reason = m_failure_reason;

    return m_status;
}

RkBool ServiceBase::InitializationSucceeded() const noexcept
{
    return m_status == EInitializationStatus::Succeeded;
}
