
#pragma once

#include "Build/Attributes/EmptyBases.hpp"
#include "Types/Units/Distance/Distance.hpp"
#include "Types/Units/Distance/EDistanceUnit.hpp"

#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorNormalization.hpp"
#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorCross.hpp"
#include "Maths/Vector/Operations/VectorSlerp.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"
#include "Maths/Vector/Operations/VectorDot.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

/**
 * \brief Two dimensional distance vector
 */
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<2, Distance<TDistanceUnit>> final:
    VectorNormalization<2, Distance<TDistanceUnit>>,
    VectorOperators    <2, Distance<TDistanceUnit>>,
    VectorMinMax       <2, Distance<TDistanceUnit>>,
    VectorLength       <2, Distance<TDistanceUnit>>,
    VectorCross        <2, Distance<TDistanceUnit>>,
    VectorSlerp        <2, Distance<TDistanceUnit>>,
    VectorLerp         <2, Distance<TDistanceUnit>>,
    VectorDot          <2, Distance<TDistanceUnit>>
{
    #pragma region Members

    union
	{
	    Distance<TDistanceUnit> data[2];

	    struct
	    {
	        Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
	    };
		struct
	    {
	        Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0)}
	{}

    constexpr Vector(Distance<TDistanceUnit> const in_x,
                     Distance<TDistanceUnit> const in_y) noexcept:
		data {in_x, in_y}
	{}

    #pragma endregion
};

#pragma warning( pop )

using Vector2cm = Vector<2, Centimeters>;
using Vector2m  = Vector<2, Meters>;
using Vector2km = Vector<2, Kilometers>;

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<2, Distance<TDistanceUnit>>>
{
	static constexpr Vector<2, Distance<TDistanceUnit>> one {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> zero {
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> up {
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> down {
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F)
	};
};

END_RUKEN_NAMESPACE