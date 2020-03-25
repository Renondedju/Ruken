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

#include <string>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Resource Identifier class
 * 
 * A resource identifier is a unique key allowing the identification of a resource.
 * This key is hash-able for fast unordered map access.
 */
struct ResourceIdentifier
{
    #pragma region Variables

    std::string name;

    #pragma endregion

    #pragma region Constructors

    ResourceIdentifier(std::string const& in_name)              noexcept;
    ResourceIdentifier(ResourceIdentifier const& in_copy) noexcept;
    ResourceIdentifier(ResourceIdentifier&&         in_move) noexcept;
    ~ResourceIdentifier() = default;
	
    #pragma endregion

    #pragma region Operators

    /**
    * \brief Converts the ResourceIdentifier to a string representation
    * \return std::string representation
    */
    explicit operator std::string() const noexcept;

    ResourceIdentifier& operator=(ResourceIdentifier const& in_copy) noexcept;
    ResourceIdentifier& operator=(ResourceIdentifier&&        in_move) noexcept;

    DAEbool operator==(ResourceIdentifier const& in_other) const noexcept;

    #pragma endregion
};

END_DAEMON_NAMESPACE

namespace std
{
    // Hash support for ResourceIdentifier
    template<>
    struct hash<DAEMON_NAMESPACE::ResourceIdentifier> 
    {
        size_t operator()(DAEMON_NAMESPACE::ResourceIdentifier const& in_identifier) const noexcept
        {
            return std::hash<std::string>()(in_identifier.name);
        }
    };
}