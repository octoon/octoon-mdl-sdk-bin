/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/mdl_d3d12/gltf.h

#ifndef MDL_D3D12_GLTF_H
#define MDL_D3D12_GLTF_H

#include "common.h"
#include "scene.h"

namespace mi { namespace examples { namespace mdl_d3d12
{
    class Loader_gltf : public IScene_loader
    {
    public:
        virtual ~Loader_gltf() = default;

        bool load(const std::string& file_name, const Scene_options& options) override;
        const Scene& get_scene() const override { return m_scene; }

        // replace all materials in the scene by one specified on the command line
        void replace_all_materials(const std::string& material_name);

    private:
        Scene m_scene;
    };

}}} // mi::examples::mdl_d3d12
#endif
