
template <typename TResource_Type>
Handle<TResource_Type>::Handle(Handle const& in_copy) noexcept:
    m_manifest {in_copy.m_manifest}
{
    if (m_manifest)
        ++m_manifest->reference_count;
}

template <typename TResource_Type>
Handle<TResource_Type>::Handle(Handle&& in_move) noexcept:
    m_manifest {std::forward<ResourceManifest*>(in_move.m_manifest)}
{
    if (m_manifest)
        ++m_manifest->reference_count;
}

template <typename TResource_Type>
Handle<TResource_Type>::~Handle()
{
    if (m_manifest)
        --m_manifest->reference_count;
}

template <typename TResource_Type>
Handle<TResource_Type>::Handle(ResourceManifest* in_manifest):
    m_manifest {in_manifest}
{
    if (m_manifest)
        ++m_manifest->reference_count;
}

template <typename TResource_Type>
TResource_Type* Handle<TResource_Type>::Get() noexcept
{
    if (!m_manifest)
        return nullptr;

    return static_cast<TResource_Type*>(m_manifest->data.load(std::memory_order_acquire));
}

template <typename TResource_Type>
TResource_Type const* Handle<TResource_Type>::Get() const noexcept
{
    if (!m_manifest)
        return nullptr;

    return static_cast<TResource_Type*>(m_manifest->data.load(std::memory_order_acquire));
}

template <typename TResource_Type>
EResourceStatus Handle<TResource_Type>::Status() const noexcept
{
    if (!m_manifest)
        return EResourceStatus::Invalid;

    return m_manifest->status.load(std::memory_order_acquire);
}

template <typename TResource_Type>
RkBool Handle<TResource_Type>::Available() const noexcept
{
    if (!m_manifest)
        return false;

    return m_manifest->status.load(std::memory_order_acquire) == EResourceStatus::Loaded;
}

template <typename TResource_Type>
RkBool Handle<TResource_Type>::Valid() const noexcept
{
    if (!m_manifest)
        return false;

    return m_manifest->status.load(std::memory_order_acquire) != EResourceStatus::Invalid;
}

template <typename TResource_Type>
ResourceManifest::ReferenceCountType Handle<TResource_Type>::ReferenceCount() const noexcept
{
    if (!m_manifest)
        return 0;

    return m_manifest->reference_count.load(std::memory_order_acquire);
}

template <typename TResource_Type>
RkBool Handle<TResource_Type>::WaitForValidity(RkFloat in_timeout) const noexcept
{
    // No manifest, cannot wait for anything (this avoid infinite loops in case of a problem)
    if (!m_manifest)
        return false;

    //TODO("Jul 23 2019", "Implement the handle timeout");
    while (!Available())
    {
        // If something wrong happened, then stopping the wait here and notifying the user
        if (m_manifest->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
            return false;

        std::this_thread::yield();
    }

    return true;
}

template <typename TResource_Type>
EResourceGCStrategy const& Handle<TResource_Type>::GCStrategy() const noexcept
{
    return m_manifest->gc_strategy.load(std::memory_order_acquire);
}

template <typename TResource_Type>
EResourceGCStrategy Handle<TResource_Type>::GCStrategy(EResourceGCStrategy const in_gc_strategy) const noexcept
{
    m_manifest->gc_strategy.store(in_gc_strategy, std::memory_order_release);
    return in_gc_strategy;
}

template <typename TResource_Type>
Handle<TResource_Type>& Handle<TResource_Type>::operator=(ResourceManifest* in_manifest) noexcept
{
    // Old manifest
    if (m_manifest)
        --m_manifest->reference_count;

    m_manifest = in_manifest;

    // New manifest
    if (m_manifest)
        ++m_manifest->reference_count;

    return *this;
}

template <typename TResource_Type>
Handle<TResource_Type>& Handle<TResource_Type>::operator=(Handle const& in_copy) noexcept
{
        // Old manifest
    if (m_manifest)
        --m_manifest->reference_count;

    m_manifest = in_copy.m_manifest;

    // New manifest
    if (m_manifest)
        ++m_manifest->reference_count;

    return *this;
}

template <typename TResource_Type>
Handle<TResource_Type>& Handle<TResource_Type>::operator=(Handle&& in_move) noexcept
{
    // Old manifest
    if (m_manifest)
        --m_manifest->reference_count;

    m_manifest = std::forward<ResourceManifest*>(in_move.m_manifest);

    // New manifest
    if (m_manifest)
        ++m_manifest->reference_count;

    return *this;
}

template <typename TResource_Type>
template<typename TDerived>
Handle<TResource_Type>::operator Handle<TDerived>() const
{
    static_assert(std::is_base_of<TDerived, TResource_Type>::value);
    return Handle<TDerived>(m_manifest);
}