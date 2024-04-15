
#pragma once

#include <string>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

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

    RkBool operator==(ResourceIdentifier const& in_other) const noexcept;

    #pragma endregion
};

END_RUKEN_NAMESPACE

namespace std
{
    // Hash support for ResourceIdentifier
    template<>
    struct hash<RUKEN_NAMESPACE::ResourceIdentifier> 
    {
        size_t operator()(RUKEN_NAMESPACE::ResourceIdentifier const& in_identifier) const noexcept
        {
            return std::hash<std::string>()(in_identifier.name);
        }
    };
}