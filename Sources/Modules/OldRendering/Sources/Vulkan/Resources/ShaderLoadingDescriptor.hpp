
#pragma once

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

struct ShaderLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        RkChar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit ShaderLoadingDescriptor(RkChar const* in_path) noexcept;

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

END_RUKEN_NAMESPACE