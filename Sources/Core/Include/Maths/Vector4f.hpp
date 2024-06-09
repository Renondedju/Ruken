
#pragma once

#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

template <>
[[deprecated("Just used to avoid compliation error with colors for now")]]
struct Vector<4, RkFloat>
{
    union 
    {
        RkFloat data[4];
        struct
        {
            RkFloat x;
            RkFloat y;
            RkFloat z;
            RkFloat w;
        };
    };    
};

using Vector4f = Vector<4, RkFloat>;

END_RUKEN_NAMESPACE