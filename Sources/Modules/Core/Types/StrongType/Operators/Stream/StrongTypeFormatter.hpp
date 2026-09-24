#pragma once

#include "Types/StrongType/StrongTypeSuffix.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Strong type formatter.
 * This class is meant to be able to specialize std::formatter for your given strong type.
 * @tparam TStrongType Strong type to format.
 */
template <typename TStrongType, typename TChar>
struct StrongTypeFormatter : std::formatter<typename TStrongType::TUnderlying, TChar>
{
	template<typename TFormatContext>
	TFormatContext::iterator format(TStrongType const& in_type, TFormatContext& in_ctx) const
	{
		using TBase = TStrongType::TUnderlying;

		return std::format_to(in_ctx.out(), "{}{}", static_cast<TBase>(in_type), StrongTypeSuffix<TStrongType>::suffix);
	}
};

END_RUKEN_NAMESPACE