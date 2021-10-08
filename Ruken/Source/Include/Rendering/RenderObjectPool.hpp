#pragma once

#include <deque>

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

template<typename T>
class RenderObjectPool
{
    protected:

        RenderDevice* m_device;

        std::deque<T> m_objects;

        virtual RkVoid Grow() = 0;

    public:

        #pragma region Constructors

        RenderObjectPool(RenderDevice* in_device) noexcept;

        RenderObjectPool(RenderObjectPool const& in_copy) = delete;
        RenderObjectPool(RenderObjectPool&&      in_move) = delete;

        virtual ~RenderObjectPool() = default;

        #pragma endregion

        #pragma region Methods

        T      Request();
        RkVoid Release(T&& in_object);

        virtual RkVoid Reset() = 0;

        #pragma endregion

        #pragma region Operators

        RenderObjectPool& operator=(RenderObjectPool const& in_copy) = delete;
        RenderObjectPool& operator=(RenderObjectPool&&      in_move) = delete;

        #pragma endregion
};

template <typename T>
RenderObjectPool<T>::RenderObjectPool(RenderDevice* in_device) noexcept:
    m_device {in_device}
{

}

template <typename T>
T RenderObjectPool<T>::Request()
{
    if (m_objects.empty())
        Grow();

    T object = m_objects.front();

    m_objects.pop_front();

    return object;
}

template <typename T>
RkVoid RenderObjectPool<T>::Release(T&& in_object)
{
    m_objects.emplace_back(std::move(in_object));
}

END_RUKEN_NAMESPACE