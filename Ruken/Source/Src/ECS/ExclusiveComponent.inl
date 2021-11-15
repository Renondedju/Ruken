
template <ComponentFieldType ... TFields>
ExclusiveComponent<TFields...>::ExclusiveComponent() noexcept:
    ComponentBase {nullptr}
{ }

#pragma warning(push)
#pragma warning(disable : 4702) // unreachable code

template <ComponentFieldType ... TFields>
RkSize ExclusiveComponent<TFields...>::EnsureStorageSpace(RkSize) noexcept
{
    RUKEN_ASSERT_MESSAGE(false, "This method should never be called on an ExclusiveComponent");

    return 0ULL;
}

#pragma warning(pop)