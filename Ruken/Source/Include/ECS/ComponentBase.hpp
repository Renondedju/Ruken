
#pragma once

// Used by the 'RUKEN_INTERNAL_DECLARE_COMPONENT' macro
#include "Meta/Meta.hpp"
#include "Meta/TupleApply.hpp"
#include "Meta/MetaForeach.hpp"
#include "Meta/TupleRemoveLast.hpp"

#include "Build/Namespace.hpp"
#include "ECS/ComponentCounter.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief Base class of the Component class, this is simply used to store components as they might have templates
 */
class ComponentBase : public ComponentCounter
{
    protected:

        #pragma region Members

        // Owning archetypes are only required for components that live in archetypes
        // Witch isn't the case for exclusive components, that lives in the entity admin
        //
        // Components that requires an archetype to exist will only expose references
        Archetype const* m_owning_archetype {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype
         */
        ComponentBase(Archetype const* in_owning_archetype) noexcept;

        ComponentBase(ComponentBase const& in_copy) = default;
        ComponentBase(ComponentBase&&      in_move) = default;
        ~ComponentBase() override                   = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        [[nodiscard]]
        virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept = 0;

        #pragma endregion

        #pragma region Operators

        ComponentBase& operator=(ComponentBase const& in_copy) = default;
        ComponentBase& operator=(ComponentBase&&      in_move) = default;

        #pragma endregion
};

// Helper macros 
#define RUKEN_INTERNAL_CREATE_FIELD(in_name, in_type) struct in_name : Field<RUKEN_EXPAND in_type> {};
#define RUKEN_INTERNAL_PREPARE_FIELD(in_value) RUKEN_INTERNAL_CREATE_FIELD in_value
#define RUKEN_INTERNAL_PREPARE_FIELD_TEMPLATE(in_value) RUKEN_EXTRACT_FIRST in_value,

/**
 * \brief Declares a ECS component
 * \param in_component_name Name of the component class
 * \param in_component_type Type of the component to declare
 * \param ... Fields of the component. Must be declared with the "RUKEN_DECLARE_FIELD" macro
 */
#define RUKEN_INTERNAL_DECLARE_COMPONENT(in_component_name, in_component_type, ...) \
    /* Creating the base internal class that will hold every field */ \
    struct in_component_name; \
    class RUKEN_GLUE(Internal, in_component_name) { \
        template <typename TType> using Field = ComponentField<TType, in_component_name>; \
        public: \
            RUKEN_FOR_EACH(RUKEN_INTERNAL_PREPARE_FIELD, __VA_ARGS__) \
            /* This weird manipulation allows to remove the garbage trailing comma that comes from the FOREACH macro manipulation */ \
            using FieldsTuple = TupleRemoveLast<std::tuple<RUKEN_FOR_EACH(RUKEN_INTERNAL_PREPARE_FIELD_TEMPLATE, __VA_ARGS__) void>>::Type; \
    }; \
    /* Then declaring the actual component type. By making it inheriting from the internal component we can make it both
     * hold the fields types as well as inheriting from the base component type while passing those field types to the base
     * Component class. This also avoids having to deal with weird crtp code later down the line.
     */ \
    using RUKEN_GLUE(in_component_name, Inheritance) = TupleApply<in_component_type, RUKEN_GLUE(Internal, in_component_name)::FieldsTuple>::Type; \
    struct in_component_name final : RUKEN_GLUE(in_component_name, Inheritance), RUKEN_GLUE(Internal, in_component_name) \
    { \
        using RUKEN_GLUE(in_component_name, Inheritance)::in_component_type; \
        using RUKEN_GLUE(in_component_name, Inheritance)::operator=; \
    }


END_RUKEN_NAMESPACE
