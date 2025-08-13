#pragma once

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept CAwaitable = requires
{
	typename TType::operator co_await;
};

END_RUKEN_NAMESPACE