
#pragma once

#include "Build/Namespace.hpp"

#include "Meta/Assert.hpp"

#include "ECS/ComponentBase.hpp"
#include "ECS/ComponentLayout.hpp"

#include "ECS/Safety/ViewType.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A component is a simple data container which contains no behavior, no code logic to transform its data.
 * \tparam TFields Fields of the component
 *
 * \warning This component cannot and should not be inherited from, instead,
 *          create a type alias of the component containing all the required fields
 */
template <ComponentFieldType... TFields>
class SparseComponent final : public ComponentBase
{
    RUKEN_STATIC_ASSERT(sizeof...(TFields) > 0, "A component must have at least one field, use a TagComponent instead.");

    public:
        
        using Layout = ComponentLayout<TFields...>;

        /**
         * \brief Returns the container type for a single field
         * \tparam TField Field to get the container type of
         */
        template <ComponentFieldType TField>
        using FieldContainerType = std::tuple_element_t<Layout::template FieldIndex<TField>::value, typename Layout::ContainerType>;

    private:

        #pragma region Members

        // Storage of the component
        typename Layout::ContainerType m_storage {};

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype
         */
        SparseComponent(Archetype const& in_owning_archetype) noexcept;

        SparseComponent(SparseComponent const& in_copy) = default;
        SparseComponent(SparseComponent&&      in_move) = default;
        virtual ~SparseComponent() override       = default;

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
        virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

        /**
         * \brief Returns a view containing all the requested fields
         * \tparam TView View type
         * \return View containing all the requested fields
         */
        template <ViewType TView>
        [[nodiscard]] TView GetView() noexcept;

        /**
         * \brief Returns a view containing all the requested fields
         * \note If the component is constant, this overload ensures that any view referencing it is readonly
         * \tparam TView View type
         * \return View containing all the requested fields
         */
        template <ReadonlyViewType TView>
        [[nodiscard]] TView GetView() const noexcept;

        #pragma endregion 

        #pragma region Operators

        SparseComponent& operator=(SparseComponent const& in_copy) = default;
        SparseComponent& operator=(SparseComponent&&      in_move) = default;

        #pragma endregion
};

#include "ECS/SparseComponent.inl"

/**
 * \brief Shorthand to declare a component named "in_component_name"
 * \param in_component_name Name of the component
 * \param ... Fields of the component. Theses must inherit from the ComponentField class
 */
#define RUKEN_DEFINE_COMPONENT(in_component_name, ...) using in_component_name = SparseComponent<__VA_ARGS__>

END_RUKEN_NAMESPACE