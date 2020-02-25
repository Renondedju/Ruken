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

template <typename TSystem>
RkVoid EntityAdmin::CreateSystem() noexcept
{
    m_systems.emplace_back(new TSystem());
}

template <typename... TComponents>
Archetype* EntityAdmin::CreateArchetype() noexcept
{
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    // Creating the actual instance
    Archetype* new_archetype = new Archetype();
    *new_archetype = Archetype::CreateArchetype<TComponents...>();

    m_archetypes[targeted_fingerprint] = new_archetype;

    // Setup
    for (ComponentSystemBase* system: m_systems)
        if (system->GetQuery().Match(*new_archetype))
            system->AddReferenceGroup(*new_archetype);

    return new_archetype; 
}

template <typename... TComponents>
EntityID EntityAdmin::CreateEntity() noexcept
{
    // Looking for the archetype of the entity
    ArchetypeFingerprint const targeted_fingerprint = ArchetypeFingerprint::CreateFingerPrintFrom<TComponents...>();

    Archetype* target_archetype;

    // If we didn't found any corresponding archetypes, creating it
    if (m_archetypes.find(targeted_fingerprint) == m_archetypes.end())
        target_archetype = CreateArchetype<TComponents...>();
    else
        target_archetype = m_archetypes[targeted_fingerprint];

    return target_archetype->CreateEntity();
}
