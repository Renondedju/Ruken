#pragma once

BEGIN_RUKEN_NAMESPACE

template <IsComponent... TComponents>
ComponentFingerprint ComponentFingerprint::CreateFingerPrintFrom() noexcept
{
    ComponentFingerprint fingerprint {};

    (fingerprint.Set(
        static_cast<RkSize>(ComponentIDFactory::StaticID<TComponents>())
    ), ...);

    return fingerprint;
}

END_RUKEN_NAMESPACE