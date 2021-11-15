
#include "Resource/ResourceIdentifier.hpp"

USING_RUKEN_NAMESPACE

ResourceIdentifier::ResourceIdentifier(std::string const& in_name) noexcept:
    name {in_name}
{}

ResourceIdentifier::ResourceIdentifier(ResourceIdentifier const& in_copy) noexcept
{
    name = in_copy.name;
}

ResourceIdentifier::ResourceIdentifier(ResourceIdentifier&& in_move) noexcept
{
    name = std::forward<std::string>(in_move.name);
}

ResourceIdentifier::operator std::string() const noexcept
{
    return name;
}

ResourceIdentifier& ResourceIdentifier::operator=(ResourceIdentifier const& in_copy) noexcept
{
    name = in_copy.name;

    return *this;
}

ResourceIdentifier& ResourceIdentifier::operator=(ResourceIdentifier&& in_move) noexcept
{
    name = std::forward<std::string>(in_move.name);

    return *this;
}

RkBool ResourceIdentifier::operator==(ResourceIdentifier const& in_other) const noexcept
{
    return name == in_other.name;
}
