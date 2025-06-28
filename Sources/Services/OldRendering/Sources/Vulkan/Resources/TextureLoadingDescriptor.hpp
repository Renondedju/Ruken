
#pragma once

#include <functional>

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;

struct TextureLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::reference_wrapper<Renderer const> renderer;

        RkChar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit TextureLoadingDescriptor(Renderer const& in_renderer, RkChar const* in_path) noexcept;

        TextureLoadingDescriptor(TextureLoadingDescriptor const&    in_copy) = default;
        TextureLoadingDescriptor(TextureLoadingDescriptor&&         in_move) = default;

        ~TextureLoadingDescriptor() = default;

        #pragma endregion

        #pragma region Operators

        TextureLoadingDescriptor& operator=(TextureLoadingDescriptor const& in_copy) = default;
        TextureLoadingDescriptor& operator=(TextureLoadingDescriptor&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE