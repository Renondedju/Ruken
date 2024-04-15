
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/Units/Pixels.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorOperators.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

/**
 * \brief Three dimensional pixel vector
 */
template <>
struct RUKEN_EMPTY_BASES Vector<3, Pixels> final:
    VectorOperators<3, Pixels>,
    VectorMinMax   <3, Pixels>,
    VectorLerp     <3, Pixels>
{
    #pragma region Members

    union
	{
	    Pixels data[3];

	    struct
	    {
	        Pixels x;
			Pixels y;
			Pixels z;
	    };
		struct
	    {
	        Pixels width;
			Pixels height;
			Pixels depth;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {0_px, 0_px, 0_px}
	{}

	constexpr Vector(Pixels const in_width, Pixels const in_height, Pixels const in_depth) noexcept:
		data {in_width, in_height, in_depth}
	{}

    #pragma endregion
};

#pragma warning( pop )

using Vector3px = Vector<3, Pixels>;

END_RUKEN_NAMESPACE