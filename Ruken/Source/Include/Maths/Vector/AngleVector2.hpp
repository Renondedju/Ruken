
#pragma once

#include "Types/Units/Angle/Angle.hpp"
#include "Build/Attributes/EmptyBases.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorSlerp.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

/**
 * \brief Two dimensional angle vector
 */
template <EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES Vector<2, Angle<TAngleUnit>> final:
    VectorOperators<2, Angle<TAngleUnit>>,
    VectorMinMax   <2, Angle<TAngleUnit>>,
    VectorSlerp    <2, Angle<TAngleUnit>>,
    VectorLerp     <2, Angle<TAngleUnit>>
{
    #pragma region Members

    union
	{
	    Angle<TAngleUnit> data[2];

	    struct
	    {
	        Angle<TAngleUnit> x;
			Angle<TAngleUnit> y;
	    };
		struct
	    {
	        Angle<TAngleUnit> pitch;
			Angle<TAngleUnit> yaw;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Angle<TAngleUnit>(0), Angle<TAngleUnit>(0)}
	{}

    constexpr Vector(Angle<TAngleUnit> const in_pitch, Angle<TAngleUnit> const in_yaw) noexcept:
		data {in_pitch, in_yaw}
	{}

    #pragma endregion
};

#pragma warning( pop )

using Vector2deg = Vector<2, Degrees>;
using Vector2rad = Vector<2, Radians>;

END_RUKEN_NAMESPACE