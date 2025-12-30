#pragma once

template <IsComponent... TComponents>
ComponentFingerprint ComponentFingerprint::CreateFingerPrintFrom() noexcept
{
    ComponentFingerprint fingerprint {};

    (fingerprint.Add(TComponents::GetId()), ...);

    return fingerprint;
}