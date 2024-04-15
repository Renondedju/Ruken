
#pragma once

#include "Build/Attributes/EmptyBases.hpp"
#include "Types/Units/Distance/Distance.hpp"
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
 * \brief Three dimensional distance vector
 */
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<3, Distance<TDistanceUnit>> final:
    VectorNormalization<3, Distance<TDistanceUnit>>,
    VectorOperators    <3, Distance<TDistanceUnit>>,
    VectorMinMax       <3, Distance<TDistanceUnit>>,
    VectorLength       <3, Distance<TDistanceUnit>>,
    VectorCross        <3, Distance<TDistanceUnit>>,
    VectorSlerp        <3, Distance<TDistanceUnit>>,
    VectorLerp         <3, Distance<TDistanceUnit>>,
    VectorDot          <3, Distance<TDistanceUnit>>
{
    #pragma region Members

    union
	{
	    Distance<TDistanceUnit> data[3];

	    struct
	    {
	        Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
			Distance<TDistanceUnit> z;
	    };
		struct
	    {
	        Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
			Distance<TDistanceUnit> depth;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0)}
	{}

    constexpr Vector(Distance<TDistanceUnit> const in_x,
                     Distance<TDistanceUnit> const in_y,
                     Distance<TDistanceUnit> const in_z) noexcept:
		data {in_x, in_y, in_z}
	{}

    #pragma endregion
};

#pragma warning( pop )

using Vector3cm = Vector<3, Centimeters>;
using Vector3m  = Vector<3, Meters>;
using Vector3km = Vector<3, Kilometers>;

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<3, Distance<TDistanceUnit>>>
{
	static constexpr Vector<3, Distance<TDistanceUnit>> one {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> zero {
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> up {
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> down {
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> forward {
        Distance<TDistanceUnit>(0.0F),
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> backward {
        Distance<TDistanceUnit>( 0.0F),
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F)
	};
};

END_RUKEN_NAMESPACE