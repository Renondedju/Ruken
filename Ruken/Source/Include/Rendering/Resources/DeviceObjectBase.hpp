#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class DeviceObjectBase
{
    protected:

        RenderDevice* m_device;

    public:

        #pragma region Constructors

        DeviceObjectBase(RenderDevice* in_device) noexcept : m_device {in_device} {}

        DeviceObjectBase(DeviceObjectBase const& in_copy) = default;
        DeviceObjectBase(DeviceObjectBase&&      in_move) = default;

        virtual ~DeviceObjectBase() = 0;

        #pragma endregion

        #pragma region Operators

        DeviceObjectBase& operator=(DeviceObjectBase const& in_copy) = default;
        DeviceObjectBase& operator=(DeviceObjectBase&&      in_move) = default;

        #pragma endregion
};

inline DeviceObjectBase::~DeviceObjectBase() = default;

END_RUKEN_NAMESPACE