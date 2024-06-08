
#pragma once

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

struct MaterialLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        RkChar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit MaterialLoadingDescriptor(RkChar const* in_path) noexcept;

        MaterialLoadingDescriptor(MaterialLoadingDescriptor const&  in_copy) = default;
        MaterialLoadingDescriptor(MaterialLoadingDescriptor&&       in_move) = default;

        ~MaterialLoadingDescriptor() = default;

        #pragma endregion

        #pragma region Operators

        MaterialLoadingDescriptor& operator=(MaterialLoadingDescriptor const&   in_copy) = default;
        MaterialLoadingDescriptor& operator=(MaterialLoadingDescriptor&&        in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE