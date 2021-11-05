
#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This struct contains the parameters used when creating a new window.
 * \note  It can also be retrieved from an existing one.
 */
struct WindowParams
{
    /**
     * \brief The UTF-8 encoded window title.
     */
    RkChar const* name {"NONE"};

    /**
     * \brief The position, in screen coordinates, of the upper-left corner of the content area of the window.
     */
    vk::Offset2D position {};

    /**
     * \brief The size, in screen coordinates, of the content area of the window.
     */
    vk::Extent2D size {};

    /**
     * \brief The opacity (or alpha) value is a positive finite number between 0 and 1.
     * \note  0 is fully transparent and 1 is fully opaque.
     */
    RkFloat opacity {1.0F};

    RkBool fullscreen              {false};
    RkBool resizable               {true};
    RkBool visible                 {true};
    RkBool decorated               {false};
    RkBool focused                 {false};
    RkBool auto_iconified          {false};
    RkBool floating                {false};
    RkBool maximized               {false};
    RkBool cursor_centered         {false};
    RkBool transparent_framebuffer {false};
    RkBool focused_on_show         {false};
    RkBool scale_to_monitor        {false};
};

END_RUKEN_NAMESPACE