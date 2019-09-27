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

#include "Containers/Tuple.hpp"
#include "ECS/EComponentStatus.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Containers/Layout/DataLayoutItem.hpp"

BEGIN_DAEMON_NAMESPACE

template<typename... TTypes>
class ComponentItemBase : DataLayoutItem<TTypes..., EComponentStatus>
{
    protected:

        template<DAEsize TIndex>
        constexpr auto& Get()       { return std::get<TIndex>(*this); }

        template<DAEsize TIndex>
        constexpr auto& Get() const { return std::get<TIndex>(*this); }

    public:

        // Constructors
        using Tuple<TTypes...>::tuple;

        /**
         * \brief Component status getter/setter
         * \return Component status
         */
        auto& GetComponentStatus()       { return std::get<std::tuple_size_v<Tuple<TTypes...>> - 1> (*this); }
        auto  GetComponentStatus() const { return std::get<std::tuple_size_v<Tuple<TTypes...>> - 1> (*this); }
};

END_DAEMON_NAMESPACE