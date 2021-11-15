
#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderTechnique
{
    private:

        #pragma region Members

        

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderTechnique() = default;

        RenderTechnique(RenderTechnique const&  in_copy) = delete;
        RenderTechnique(RenderTechnique&&       in_move) = delete;

        ~RenderTechnique() = default;

        #pragma endregion

        #pragma region Methods



        #pragma endregion

        #pragma region Operators

        RenderTechnique& operator=(RenderTechnique const&   in_copy) = delete;
        RenderTechnique& operator=(RenderTechnique&&        in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE