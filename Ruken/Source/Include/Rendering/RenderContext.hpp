#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;

class RenderContext
{
    private:

        #pragma region Members

        Logger* m_logger;

        vk::DynamicLoader          m_loader;
        vk::Instance               m_instance;
        vk::DebugUtilsMessengerEXT m_messenger;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderContext(Logger* in_logger = nullptr) noexcept;

        RenderContext(RenderContext const& in_copy) = delete;
        RenderContext(RenderContext&&      in_move) = delete;

        ~RenderContext() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::Instance const& GetInstance() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderContext& operator=(RenderContext const& in_copy) = delete;
        RenderContext& operator=(RenderContext&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE
