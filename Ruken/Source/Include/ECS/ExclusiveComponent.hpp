
#pragma once

#include <tuple>

#include "Meta/Assert.hpp"
#include "Build/Namespace.hpp"

#include "ECS/ComponentBase.hpp"
#include "ECS/Meta/FieldHelper.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Exclusive components are a type of component where only one instance of the component exists in the whole ECS
 */
template <ComponentFieldType... TFields>
class ExclusiveComponent : public ComponentCounter
{
    RUKEN_STATIC_ASSERT(sizeof...(TFields) > 0, "An exclusive component must have at least one field, use a TagComponent instead.");

    using Helper = FieldHelper<TFields...>;

    private:

        #pragma region Members

        std::tuple<typename TFields::Type...> m_members;

        #pragma endregion

    public:

        #pragma region Constructors

        ExclusiveComponent()                                  = default;
        ExclusiveComponent(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent(ExclusiveComponent&&      in_move) = default;
        ~ExclusiveComponent() override                        = default;

        #pragma endregion

        #pragma region Methods

        RUKEN_DEFINE_COMPONENT_ID_DECLARATION

        /**
         * \brief Fetches a field from the component
         * \tparam TField Field to fetch
         * \return Reference to the field
         */
        template <ComponentFieldType TField> requires Helper::template FieldExists<TField>::value
        [[nodiscard]] typename TField::Type& Fetch() noexcept
        { return std::get<Helper::template FieldIndex<TField>::value>(m_members); }

        /**
         * \brief Fetches a field from the component
         * \tparam TField Field to fetch
         * \return Constant reference to the field
         */
        template <ComponentFieldType TField> requires Helper::template FieldExists<TField>::value
        [[nodiscard]] typename TField::Type const& Fetch() const noexcept
        { return std::get<Helper::template FieldIndex<TField>::value>(m_members); }

        #pragma endregion

        #pragma region Operators

        ExclusiveComponent& operator=(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent& operator=(ExclusiveComponent&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief Declares an ECS component
 * \param in_component_name Name of the component class
 * \param ... Fields of the component. Must be declared with the "RUKEN_DECLARE_FIELD" macro
 */
#define RUKEN_DECLARE_EXCLUSIVE_COMPONENT(in_component_name, ...) RUKEN_INTERNAL_DECLARE_COMPONENT(in_component_name, ExclusiveComponent, __VA_ARGS__)

END_RUKEN_NAMESPACE