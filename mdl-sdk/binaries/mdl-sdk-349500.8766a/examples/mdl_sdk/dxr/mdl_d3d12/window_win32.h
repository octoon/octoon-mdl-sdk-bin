/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/mdl_d3d12/window_win32.h

#ifndef MDL_D3D12_WINDOW_WIN32_H
#define MDL_D3D12_WINDOW_WIN32_H

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

    class Window_win32 : public IWindow
    {
        friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    public:
        explicit Window_win32(Base_application_message_interface& message_pump_interface);
        virtual ~Window_win32();

        int show(int nCmdShow) override;
        void close() override;
        bool has_focus() const override;

        Texture* get_back_buffer() const override;
        D3D12_CPU_DESCRIPTOR_HANDLE get_back_buffer_rtv() const override;
        bool present_back_buffer() override;

        size_t get_width() const override { return m_width; }
        size_t get_height() const override { return m_height; }
        bool resize(size_t width, size_t height, double dpi) override;

        void set_vsync(bool on) override { m_vsync = on; }
        bool get_vsync() const override { return m_vsync; }

        void set_window_mode(IWindow::Mode mode) override;
        IWindow::Mode get_window_mode() const override { return m_mode; }

        // low level interfaces to the applications message pump
        void add_message_callback(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> callback);

        // low level interfaces to application window
        HWND get_window_handle() const { return m_window_handle; }

        // get the windows main UI instance
        mi::examples::gui::Root* get_gui() final { return m_gui; };

    private:
        Base_application* m_app;
        Base_application_message_interface& m_message_pump_interface;

        HWND m_window_handle;
        size_t m_width;
        size_t m_height;
        double m_dpi;
        bool m_vsync;
        IWindow::Mode m_mode;

        ComPtr<IDXGISwapChain3> m_swap_chain;

        UINT m_swap_buffer_index;
        std::vector<Texture*> m_swap_buffers;
        std::vector<UINT64> m_swap_fence_handles;
        std::vector<Descriptor_heap_handle> m_render_target_views_heap_indices;

        std::vector<std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> m_message_callbacks;
        bool m_close;

        mi::examples::gui::Root* m_gui;
    };

}}} // mi::examples::mdl_d3d12
#endif
