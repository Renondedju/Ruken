#pragma once

#include "Build/Namespace.hpp"

#include <tuple>

BEGIN_RUKEN_NAMESPACE

/**
 * Transforms all the types of a tuple into other types using a transformer struct.
 * @tparam TTransformer Must be a templated struct declaring a type alias for the transformed type named "Type"
 * @tparam TTuple Tuple type to transform.
 * @return Transformed tuple.
 */
//template <template <typename> typename TTransformer, typename... TTuple>
//auto TupleTransform(std::tuple<TTuple...>&&) -> std::tuple<typename TTransformer<TTuple>::TType...>;

template <template <typename> typename TTransformer, typename TType>
struct TupleTransform;

template <template <typename> typename TTransformer, typename... TTuple>
struct TupleTransform<TTransformer, std::tuple<TTuple...>>
{
	using TType = std::tuple<typename TTransformer<TTuple>::TType...>;
};

/**
 * Transforms all the types of a tuple into other types using a transformer struct.
 * @tparam TTransformer Must be a templated struct declaring a type alias for the transformed type named "Type"
 * @tparam TTuple Tuple type to transform.
 * @return Transformed tuple.
 */
template <template <typename> typename TTransformer, typename TTuple>
using TupleTransformT = TupleTransform<TTransformer, TTuple>::TType;

END_RUKEN_NAMESPACE