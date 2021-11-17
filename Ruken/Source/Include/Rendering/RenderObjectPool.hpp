#pragma once

#include <deque>

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

template<typename T>
class RenderObjectPool
{
    protected:

        #pragma region Members

        RenderDevice* m_device;

        std::deque<T> m_objects;

        #pragma endregion

        #pragma region Methods

        virtual RkVoid Grow() = 0;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderObjectPool(RenderDevice* in_device) noexcept;

        RenderObjectPool(RenderObjectPool const& in_copy) = delete;
        RenderObjectPool(RenderObjectPool&&      in_move) = delete;

        virtual ~RenderObjectPool() = default;

        #pragma endregion

        #pragma region Methods

        T      Request();
        RkVoid Release(T const& in_object);
        RkVoid Release(T&&      in_object);

        std::vector<T> Request(RkSize                in_count);
        RkVoid         Release(std::vector<T> const& in_objects);
        RkVoid         Release(std::vector<T>&&      in_objects);

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
RkVoid RenderObjectPool<T>::Release(T const& in_object)
{
    m_objects.emplace_back(in_object);
}

template <typename T>
RkVoid RenderObjectPool<T>::Release(T&& in_object)
{
    m_objects.emplace_back(std::move(in_object));
}

template <typename T>
std::vector<T> RenderObjectPool<T>::Request(RkSize const in_count)
{
    std::vector<T> objects;

    while (objects.size() < in_count)
    {
        objects.emplace_back(Request());
    }

    return objects;
}

template <typename T>
RkVoid RenderObjectPool<T>::Release(std::vector<T> const& in_objects)
{
    m_objects.insert(m_objects.end(), in_objects.begin(), in_objects.end());
}

template <typename T>
RkVoid RenderObjectPool<T>::Release(std::vector<T>&& in_objects)
{
    m_objects.insert(m_objects.end(), std::make_move_iterator(in_objects.begin()), std::make_move_iterator(in_objects.end()));
}

END_RUKEN_NAMESPACE