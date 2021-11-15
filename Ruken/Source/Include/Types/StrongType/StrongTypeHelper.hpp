

#pragma once

#include "Types/StrongType/StrongType.hpp"

BEGIN_RUKEN_NAMESPACE

class StrongTypeHelper
{
    /**
     * \brief Implementation of the UnderlyingType method
     * \tparam TBaseType Base type of the StrongType
     * \tparam TUniquePhantomType Phantom type of the StrongType
     * \return Base type of the StrongType
     */
    template <typename TBaseType, typename TUniquePhantomType>
    static constexpr TBaseType UnderlyingTypeImpl(StrongType<TBaseType, TUniquePhantomType>);

    public:

        #pragma region Usings

        /**
         * \brief UnderlyingType helper method, this is used internally to get the underlying type of complex StrongType types
         * 
         * This has to be used instead of StrongType::TBase since complex strong types inherits from the StrongType class.
         * This methods avoids the issue of ill-formed types.
         * 
         * \see This wonderful article, where everything is nicely explained : https://foonathan.net/blog/2016/10/19/strong-typedefs.html
         * \tparam TStrongType Base type of the StrongType
         */
        template <typename TStrongType>
        using UnderlyingType = decltype(UnderlyingTypeImpl(std::declval<TStrongType>()));

        #pragma endregion
};

END_RUKEN_NAMESPACE