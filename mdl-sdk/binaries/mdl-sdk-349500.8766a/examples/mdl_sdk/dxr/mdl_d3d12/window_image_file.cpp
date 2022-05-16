/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

#include "window_image_file.h"
#include "texture.h"
#include "base_application.h"
#include "command_queue.h"
#include "descriptor_heap.h"
#include "mdl_sdk.h"

namespace mi { namespace examples { namespace mdl_d3d12
{

Window_image_file::Window_image_file(
    Base_application_message_interface& message_pump_interface,
    std::string file_path,
    size_t iteration_count)

    : m_app(message_pump_interface.get_application())
    , m_message_pump_interface(message_pump_interface)
    , m_close(false)
    , m_file_path(file_path)
    , m_iteration_count(iteration_count)
{
    auto options = m_app->get_options();
    m_width = options->window_width;
    m_height = options->window_height;

    // select the back buffer format depending on the output file type
    DXGI_FORMAT output_format = DXGI_FORMAT_R8G8B8A8_UNORM;
    if (mi::examples::strings::ends_with(m_file_path, ".exr") ||
        mi::examples::strings::ends_with(m_file_path, ".hdr"))
            output_format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // create texture buffer
    m_back_buffer = Texture::create_texture_2d(
        m_app, GPU_access::render_target,
        m_width, m_height, output_format,
        "BackBuffer");

    // create render target view
    m_back_buffer_rtv = m_app->get_render_target_descriptor_heap()->reserve_views(1);
    if (!m_app->get_render_target_descriptor_heap()->create_render_target_view(
        m_back_buffer, m_back_buffer_rtv))
    {
        std::string msg = "Failed to create resource view for back buffer.";
        log_error(msg, SRC);
        throw(msg);
    }
}

// ------------------------------------------------------------------------------------------------

Window_image_file::~Window_image_file()
{
    delete m_back_buffer;
}

// ------------------------------------------------------------------------------------------------

int Window_image_file::show(int nCmdShow)
{
    auto command_queue = m_app->get_command_queue(D3D12_COMMAND_LIST_TYPE_DIRECT);

    log_info("iterations to run: " + std::to_string(m_iteration_count));
    size_t message_step = m_iteration_count / 10;
    {
        Timing t("rendering");
        for (size_t i = 0; i < m_iteration_count; ++i)
        {
            m_message_pump_interface.paint();
            command_queue->flush();

            if ((i + 1) % message_step == 0)
                log_info("rendering completed to " + std::to_string((i + 1) / message_step * 10));

            if (m_close)
                return 0;
        }
    }

    // commit work and make sure the result is ready
    auto command_list = command_queue->get_command_list();
    m_back_buffer->transition_to(command_list, D3D12_RESOURCE_STATE_COMMON);
    command_queue->execute_command_list(command_list);
    m_app->flush_command_queues();

    // use neuray to write the image file
    const char* format = "Rgba";
    if (m_back_buffer->get_format() == DXGI_FORMAT_R32G32B32A32_FLOAT)
        format = "Float32<4>";

    mi::base::Handle<mi::neuraylib::ICanvas> canvas(
        m_app->get_mdl_sdk().get_image_api().create_canvas(
        format,
        static_cast<mi::Uint32>(m_back_buffer->get_width()),
        static_cast<mi::Uint32>(m_back_buffer->get_height())));

    mi::base::Handle<mi::neuraylib::ITile> tile(canvas->get_tile());

    // download texture and save to output file
    if (m_back_buffer->download(tile->get_data()))
    {
        m_app->get_mdl_sdk().get_impexp_api().export_canvas(m_file_path.c_str(), canvas.get());
    }

    // keep console open in debug
    if (IsDebuggerPresent())
        system("pause");

    return 0;
}

// ------------------------------------------------------------------------------------------------

D3D12_CPU_DESCRIPTOR_HANDLE Window_image_file::get_back_buffer_rtv() const
{
    return m_back_buffer_rtv.get_cpu_handle();
}

}}} // mi::examples::mdl_d3d12
