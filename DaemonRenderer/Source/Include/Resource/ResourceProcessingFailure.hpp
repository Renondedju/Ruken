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

#include <string>
#include <exception>

#include "Config.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Resource/Enums/EResourceLoadingFailureCode.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Resource loading failure exception.
 * 
 * This exception and all the derived exceptions are handled by the resource manager.
 */
struct ResourceProcessingFailure final : std::exception
{
    #pragma region Variables

    /**
     * \brief Long description of the exception.
     * This will be logged by the resource manager
     */
    std::string description;

    /**
     * \brief Resource validity after the exception.
     * If set to true, the error message will still be displayed but the resource will be considered as valid.
     * This can be useful if a resource by default has been loaded instead.
     */
    DAEbool resource_validity;

    /**
     * \brief Error code.
     * This is a short description of the error and can be indicative of if a retry can be considered or not.
     */
    EResourceProcessingFailureCode code;

    #pragma endregion

    #pragma region Constructors

    explicit ResourceProcessingFailure (EResourceProcessingFailureCode in_code, DAEbool in_validity = false, DAEchar const* in_description = "") noexcept;

    ResourceProcessingFailure ()                                         noexcept = delete;
    ResourceProcessingFailure (ResourceProcessingFailure const& in_copy) noexcept = default;
    ResourceProcessingFailure (ResourceProcessingFailure&& in_move)         noexcept = default;
    virtual ~ResourceProcessingFailure()                                          = default;

    #pragma endregion

    #pragma region Methods
    
    char const* what() const override;

    #pragma endregion 

    #pragma region Operators

    /**
     * \brief std::string representation
     */
    [[nodiscard]]
    explicit operator std::string() const noexcept;

    ResourceProcessingFailure& operator=(ResourceProcessingFailure const& in_copy) noexcept = default;
    ResourceProcessingFailure& operator=(ResourceProcessingFailure&&      in_move) noexcept = default;

    #pragma endregion
};

END_DAEMON_NAMESPACE
