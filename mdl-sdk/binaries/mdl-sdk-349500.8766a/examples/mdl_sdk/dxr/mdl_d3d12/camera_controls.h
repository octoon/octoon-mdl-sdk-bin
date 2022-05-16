/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/camera_controls.h

#ifndef MDL_D3D12_CAMERA_CONTROLS_H
#define MDL_D3D12_CAMERA_CONTROLS_H

#include "mdl_d3d12/common.h"

namespace mi { namespace examples { namespace mdl_d3d12
{
    class Base_application;
    struct Bounding_box;
    struct Update_args;
    class Scene_node;

    // --------------------------------------------------------------------------------------------

    class Camera_controls
    {
    public:
        explicit Camera_controls(
            mdl_d3d12::Base_application* app,
            mdl_d3d12::Scene_node* node = nullptr);
        virtual ~Camera_controls() = default;

        bool update(const mdl_d3d12::Update_args& args);
        mdl_d3d12::Scene_node* get_target() { return m_target; }
        void set_target(mdl_d3d12::Scene_node* node);

        void fit_into_view(const Bounding_box& worldspace_aabb);

        float movement_speed;
        float rotation_speed;

    private:
        mdl_d3d12::Base_application* m_app;
        bool m_left_mouse_button_held;
        bool m_middle_mouse_button_held;
        bool m_right_mouse_button_held;
        int32_t m_mouse_move_start_x;
        int32_t m_mouse_move_start_y;
        mdl_d3d12::Scene_node* m_target;
        bool m_has_focus;
    };

}}} // mi::examples::mdl_d3d12
#endif
