
template <ComponentType... TComponents>
ArchetypeFingerprint ArchetypeFingerprint::CreateFingerPrintFrom() noexcept
{
    ArchetypeFingerprint fingerprint;
    (fingerprint.Add(TComponents::GetId()), ...);

    return fingerprint;
}