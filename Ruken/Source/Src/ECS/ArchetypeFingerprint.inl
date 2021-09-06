
template <AnyComponentType... TComponents>
ArchetypeFingerprint ArchetypeFingerprint::CreateFingerPrintFrom() noexcept
{
    ArchetypeFingerprint fingerprint;
    (fingerprint.Add(TComponents::GetId()), ...);

    return fingerprint;
}