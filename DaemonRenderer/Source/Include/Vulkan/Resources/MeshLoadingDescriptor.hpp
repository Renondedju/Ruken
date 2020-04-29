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

#include <functional>

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_DAEMON_NAMESPACE

class Renderer;

struct MeshLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::reference_wrapper<Renderer const> renderer;

        DAEchar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit MeshLoadingDescriptor(Renderer const& in_renderer, DAEchar const* in_path) noexcept;

        MeshLoadingDescriptor(MeshLoadingDescriptor const&  in_copy) = default;
        MeshLoadingDescriptor(MeshLoadingDescriptor&&       in_move) = default;

        ~MeshLoadingDescriptor() = default;

        #pragma endregion

        #pragma region Methods

        #pragma endregion

        #pragma region Operators

        MeshLoadingDescriptor& operator=(MeshLoadingDescriptor const&   in_copy) = default;
        MeshLoadingDescriptor& operator=(MeshLoadingDescriptor&&        in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE