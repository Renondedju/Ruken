/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

template <SystemType TSystem>
RkVoid EntityAdmin::CreateSystem() noexcept
{
    std::unique_ptr<TSystem> system = std::make_unique<TSystem>(*this);

    m_systems.emplace_back(std::move(system));
}

template <ComponentType... TComponents>
Archetype* EntityAdmin::CreateArchetype() noexcept
{
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    // Creating the actual instance
    std::unique_ptr<Archetype> new_archetype = std::make_unique<Archetype>(Tag<TComponents...>());

    // We need to get the pointer before moving it
    Archetype* archetype_ptr = new_archetype.get();
    m_archetypes[targeted_fingerprint] = std::move(new_archetype);

    // Setup
    for (std::unique_ptr<SystemBase>& system: m_systems)
        if (system->GetQuery().Match(*archetype_ptr))
            system->AddReferenceGroup(*archetype_ptr);

    return archetype_ptr; 
}

template <ComponentType... TComponents>
Entity EntityAdmin::CreateEntity() noexcept
{
    // Looking for the archetype of the entity
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    Archetype* target_archetype;

    // If we didn't found any corresponding archetypes, creating it
    if (m_archetypes.find(targeted_fingerprint) == m_archetypes.end())
        target_archetype = CreateArchetype<TComponents...>();
    else
        target_archetype = m_archetypes[targeted_fingerprint].get();

    return target_archetype->CreateEntity();
}

template <ExclusiveComponentType TComponent>
TComponent& EntityAdmin::GetExclusiveComponent() noexcept
{
    // If we didn't found any corresponding component, creating it
    if (m_exclusive_components.find(TComponent::GetId()) == m_exclusive_components.end())
        m_exclusive_components.emplace(TComponent::GetId(), std::make_unique<TComponent>());

    return *static_cast<TComponent*>(m_exclusive_components.at(TComponent::GetId()).get());
}
