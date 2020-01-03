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

#include <algorithm>

#include "ECS/ArchetypeFingerprint.hpp"

USING_DAEMON_NAMESPACE

DAEvoid ArchetypeFingerprint::AddTrait(DAEsize in_trait) noexcept
{
    DAEint64 const size    = sizeof(EFragmentContent) * 8Ui64;
    DAEint64 const section = in_trait / size;

    in_trait %= size;

    // Checking if the requested section is missing
    for (int i = 0; (section + 1) - static_cast<DAEint64>(m_fingerprint.size()) > 0ll; ++i)
        m_fingerprint.emplace_back(Bitmask<EFragmentContent>());

    m_fingerprint[section].Add(static_cast<EFragmentContent>(in_trait));
}

DAEbool ArchetypeFingerprint::IsSubsetOf(ArchetypeFingerprint const& in_other) const noexcept
{
    if (m_fingerprint.size() > in_other.m_fingerprint.size())
        return false;

    for (DAEsize index = 0; index < m_fingerprint.size(); ++index)
        if (! in_other.m_fingerprint[index].HasAll(m_fingerprint[index]))
            return false;

    return true;    
}
