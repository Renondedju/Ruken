﻿/*
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

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_DAEMON_NAMESPACE

struct ShaderLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        DAEchar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit ShaderLoadingDescriptor(DAEchar const* in_path) noexcept;

        ShaderLoadingDescriptor(ShaderLoadingDescriptor const&  in_copy) = default;
        ShaderLoadingDescriptor(ShaderLoadingDescriptor&&       in_move) = default;

        ~ShaderLoadingDescriptor() = default;

        #pragma endregion

        #pragma region Methods

        #pragma endregion

        #pragma region Operators

        ShaderLoadingDescriptor& operator=(ShaderLoadingDescriptor const&   in_copy) = default;
        ShaderLoadingDescriptor& operator=(ShaderLoadingDescriptor&&        in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE