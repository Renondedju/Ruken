
#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderView
{
    private:

        #pragma region Members

        

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderView() = default;

        RenderView(RenderView const&    in_copy) = delete;
        RenderView(RenderView&&         in_move) = delete;

        ~RenderView() = default;

        #pragma endregion

        #pragma region Methods



        #pragma endregion

        #pragma region Operators

        RenderView& operator=(RenderView const& in_copy) = delete;
        RenderView& operator=(RenderView&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE