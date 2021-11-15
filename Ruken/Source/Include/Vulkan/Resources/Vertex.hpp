
#pragma once

#include "Rendering/UV.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

struct Vertex
{
    public:

        #pragma region Members

        Vector3m position;
        Vector3m normal;
        UV       uv;

        #pragma endregion

        #pragma region Constructors and Destructor

        Vertex() = default;

        Vertex(Vertex const&    in_copy) = default;
        Vertex(Vertex&&         in_move) = default;

        ~Vertex() = default;

        #pragma endregion

        #pragma region Operators

        Vertex& operator=(Vertex const& in_copy) = default;
        Vertex& operator=(Vertex&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE