
#pragma once

#include "Meta/Assert.hpp"

#include "Maths/MinMax.hpp"
#include "ECS/ComponentBase.hpp"
#include "Containers/LinkedChunkList.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
class Component: public ComponentBase
{
    // Field helper
    using Helper = FieldHelper<TFields...>;

    RUKEN_STATIC_ASSERT(sizeof...(TFields) > 0, "A component must have at least one field, use a TagComponent instead.");

    public:

        // Creating the memory layout of the component
        template <ComponentFieldType TField> requires Helper::template FieldExists<TField>::value
        using FieldContainerType = LinkedChunkList<typename TField::Type>;

    private:

        #pragma region Members

        // Storage of the component
        std::tuple<FieldContainerType<TFields>...> m_storage {};

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Storage space helper method
         * \tparam TIds IDs from 0 to the number of fields held in the component
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the component layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        template <RkSize... TIds>
        RkSize EnsureStorageSpaceHelper(RkSize in_size, std::index_sequence<TIds...>) noexcept;

        #pragma endregion 

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype pointer
         */
        Component(Archetype const* in_owning_archetype) noexcept;

        Component(Component const& in_copy) = default;
        Component(Component&&      in_move) = default;
        ~Component() override               = default;

        #pragma endregion

        #pragma region Methods

        RUKEN_DEFINE_COMPONENT_ID_DECLARATION

        /**
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the component layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        [[nodiscard]]
        RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

        /**
         * \brief Returns a reference onto a field container
         * \tparam TField Field to look for. Must be held by the component
         * \return Field container reference
         */
        template <ComponentFieldType TField>
        [[nodiscard]]
        FieldContainerType<TField>& GetFieldContainer() noexcept;

        #pragma endregion

        #pragma region Operators

        Component& operator=(Component const& in_copy) = default;
        Component& operator=(Component&&      in_move) = default;

        #pragma endregion
};

#include "ECS/Component.inl"

/**
 * \brief Declares an ECS component
 * \param in_component_name Name of the component class
 * \param ... Fields of the component. Must be declared with the "RUKEN_DECLARE_FIELD" macro
 */
#define RUKEN_DECLARE_COMPONENT(in_component_name, ...) RUKEN_INTERNAL_DECLARE_COMPONENT(in_component_name, Component, __VA_ARGS__)

END_RUKEN_NAMESPACE