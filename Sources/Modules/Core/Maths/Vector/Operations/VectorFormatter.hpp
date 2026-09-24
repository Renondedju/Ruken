#pragma once

#include <format>

#include "Core/Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

template <RkSize TDimensions, typename TUnderlyingType, typename TChar>
struct VectorFormatter : std::range_formatter<TUnderlyingType, TChar>
{
	template<class TParseContext>
	constexpr TParseContext::iterator parse(TParseContext& in_ctx)
	{
		std::range_formatter<TUnderlyingType, TChar>::set_brackets("(", ")");

		return std::range_formatter<TUnderlyingType, TChar>::parse(in_ctx);
	}

	template<class TFormatContext>
	TFormatContext::iterator format(Vector<TDimensions, TUnderlyingType> const& in_vector, TFormatContext& in_ctx) const
	{
		return std::range_formatter<TUnderlyingType, TChar>::format(in_vector.data, in_ctx);
	}
};

END_RUKEN_NAMESPACE
