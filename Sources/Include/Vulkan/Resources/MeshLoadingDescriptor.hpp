
#pragma once

#include <functional>

#include "Types/FundamentalTypes.hpp"

#include "Resource/ResourceLoadingDescriptor.hpp"

BEGIN_RUKEN_NAMESPACE

class Renderer;

struct MeshLoadingDescriptor final : ResourceLoadingDescriptor
{
    public:

        #pragma region Members

        std::reference_wrapper<Renderer const> renderer;

        RkChar const* path;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit MeshLoadingDescriptor(Renderer const& in_renderer, RkChar const* in_path) noexcept;

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

END_RUKEN_NAMESPACE