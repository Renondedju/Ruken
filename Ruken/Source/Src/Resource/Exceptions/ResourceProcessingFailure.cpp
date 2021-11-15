
#include "Resource/ResourceProcessingFailure.hpp"

USING_RUKEN_NAMESPACE

ResourceProcessingFailure::ResourceProcessingFailure(EResourceProcessingFailureCode const in_code, RkBool const in_validity, RkChar const* in_description) noexcept:
    std::exception        {in_description},
    description            {in_description},
    resource_validity    {in_validity},
    code                {in_code}
{}

ResourceProcessingFailure::operator std::string() const noexcept
{
    return EnumToString(code) + ": " + description;
}

char const* ResourceProcessingFailure::what() const
{
    return std::exception::what();
}
