
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
 * \brief Three dimensional angle vector
 */
template <EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES Vector<3, Angle<TAngleUnit>> final:
    VectorOperators<3, Angle<TAngleUnit>>,
    VectorMinMax   <3, Angle<TAngleUnit>>,
    VectorSlerp    <3, Angle<TAngleUnit>>,
    VectorLerp     <3, Angle<TAngleUnit>>
{
    #pragma region Members

    union
	{
	    Angle<TAngleUnit> data[3];

	    struct
	    {
	        Angle<TAngleUnit> x;
			Angle<TAngleUnit> y;
			Angle<TAngleUnit> z;
	    };
		struct
	    {
	        Angle<TAngleUnit> pitch;
			Angle<TAngleUnit> yaw;
			Angle<TAngleUnit> roll;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Angle<TAngleUnit>(0),
	          Angle<TAngleUnit>(0),
	          Angle<TAngleUnit>(0)}
	{}

    constexpr Vector(Angle<TAngleUnit> const in_pitch,
                     Angle<TAngleUnit> const in_yaw,
                     Angle<TAngleUnit> const in_roll) noexcept:
		data {in_pitch, in_yaw, in_roll}
	{}

    #pragma endregion
};

#pragma warning( pop )

using Vector3deg = Vector<3, Degrees>;
using Vector3rad = Vector<3, Radians>;

END_RUKEN_NAMESPACE