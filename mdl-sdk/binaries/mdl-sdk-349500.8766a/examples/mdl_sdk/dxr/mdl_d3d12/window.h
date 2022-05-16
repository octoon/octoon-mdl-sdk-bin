/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/mdl_d3d12/window.h

#ifndef MDL_D3D12_IWINDOW_H
#define MDL_D3D12_IWINDOW_H

#include "common.h"

namespace mi { namespace examples { namespace gui
{
    class Root;
}}}

namespace mi { namespace examples { namespace mdl_d3d12
{
    class Texture;

    // ------------------------------------------------------------------------

    class IWindow
    {
    public:

        // --------------------------------------------------------------------

        enum class Mode
        {
            Windowed,
            Fullsceen,
            // Borderless_windowed /*not implemented*/
        };

        // --------------------------------------------------------------------

        virtual ~IWindow() = default;

        /// called internally in order to run the main loop
        virtual int show(int nCmdShow) = 0;

        /// closes the window after finishing the current frame
        virtual void close() = 0;

        /// returns true if the window is the foreground window
        virtual bool has_focus() const = 0;

        virtual Texture* get_back_buffer() const = 0;
        virtual D3D12_CPU_DESCRIPTOR_HANDLE get_back_buffer_rtv() const = 0;
        virtual bool present_back_buffer() = 0;


        virtual size_t get_width() const = 0;
        virtual size_t get_height() const = 0;
        virtual bool resize(size_t width, size_t height, double dpi) = 0;

        virtual void set_vsync(bool on) = 0;
        virtual bool get_vsync() const = 0;

        virtual void set_window_mode(Mode mode) = 0;
        virtual Mode get_window_mode() const = 0;

        // get the windows main UI instance
        virtual mi::examples::gui::Root* get_gui() = 0;
    };

}}} // mi::examples::mdl_d3d12
#endif
