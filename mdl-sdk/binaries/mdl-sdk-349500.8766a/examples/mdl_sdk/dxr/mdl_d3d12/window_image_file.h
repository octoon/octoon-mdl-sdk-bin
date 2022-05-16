/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/mdl_d3d12/window_image_file.h

#ifndef MDL_D3D12_WINDOW_IMAGE_FILE_H
#define MDL_D3D12_WINDOW_IMAGE_FILE_H

#include "common.h"
#include "window.h"

namespace mi { namespace examples { namespace gui
{
    class Root;
}}}

namespace mi { namespace examples { namespace mdl_d3d12
{
    class Base_application;
    class Base_application_message_interface;
    class Texture;

    // ------------------------------------------------------------------------

    class Window_image_file : public IWindow
    {
        friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    public:
        explicit Window_image_file(
            Base_application_message_interface& message_pump_interface,
            std::string file_path,
            size_t iteration_count);

        virtual ~Window_image_file();

        int show(int nCmdShow) override;
        void close() override { m_close = true; }
        bool has_focus() const override { return false; }

        Texture* get_back_buffer() const override { return m_back_buffer; }
        D3D12_CPU_DESCRIPTOR_HANDLE get_back_buffer_rtv() const override;
        bool present_back_buffer() override { return true; }

        size_t get_width() const override { return m_width; }
        size_t get_height() const override { return m_height; }

        // no support for resize
        bool resize(size_t width, size_t height, double dpi) override { return false; };

        // no support for vsync
        void set_vsync(bool on) override { }
        bool get_vsync() const override { return false; }

        // no support for window modes
        void set_window_mode(IWindow::Mode mode) override { };
        IWindow::Mode get_window_mode() const override { return IWindow::Mode::Windowed; }

        // get the windows main UI instance
        mi::examples::gui::Root* get_gui() final { return nullptr; };

    private:
        Base_application* m_app;
        Base_application_message_interface& m_message_pump_interface;

        size_t m_width;
        size_t m_height;
        double m_dpi;

        ComPtr<IDXGISwapChain3> m_swap_chain;

        Texture* m_back_buffer;
        Descriptor_heap_handle m_back_buffer_rtv;

        std::string m_file_path;
        size_t m_iteration_count;

        bool m_close;
    };

}}} // mi::examples::mdl_d3d12
#endif
