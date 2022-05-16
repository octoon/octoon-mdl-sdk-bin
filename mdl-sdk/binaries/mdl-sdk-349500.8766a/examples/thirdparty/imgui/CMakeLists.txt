#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# name of the target and the resulting library
set(PROJECT_NAME imgui)
set(PROJECT_VERSION 1.75) # 58b3e02b95b4c7c5bb9128a28c6d55546501bf93

# collect sources
set(PROJECT_SOURCES
    "imconfig.h"
    "imgui.h"
    "imgui_internal.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_widgets.cpp"
    )

if(WINDOWS)
    list(APPEND PROJECT_SOURCES 
        "imgui_impl_win32.h"
        "imgui_impl_win32.cpp"
        )

    if(MDL_ENABLE_D3D11_EXAMPLES)
        list(APPEND PROJECT_SOURCES 
            "imgui_impl_dx11.cpp"
            "imgui_impl_dx11.h"
        )
    endif()

    if(MDL_ENABLE_D3D12_EXAMPLES)
        list(APPEND PROJECT_SOURCES 
            "imgui_impl_dx12.cpp"
            "imgui_impl_dx12.h"
        )
		include_directories(
			AFTER ${MDL_DEPENDENCY_D3D12_INCLUDE} ${MDL_DEPENDENCY_DXGI_INCLUDE}
		)
    endif()
endif()

if(MACOSX)
    list(APPEND PROJECT_SOURCES 
        "imgui_impl_osx.h"
        "imgui_impl_osx.mm"
        )
endif()

if(MDL_ENABLE_OPENGL_EXAMPLES)
    list(APPEND PROJECT_SOURCES 
        "imgui_impl_glfw.cpp"
        "imgui_impl_glfw.h"
        "imgui_impl_opengl3.cpp"
        "imgui_impl_opengl3.h"
    )
endif()

# create target from template
create_from_base_preset(
    TARGET ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    SOURCES ${PROJECT_SOURCES}
    EXAMPLE
)

if(MDL_ENABLE_OPENGL_EXAMPLES)
    # add dependencies
    target_add_dependencies(TARGET ${PROJECT_NAME} 
        DEPENDS 
            opengl 
        )

    # additional definitions
    target_compile_definitions(${PROJECT_NAME} 
        PRIVATE 
            "IMGUI_IMPL_OPENGL_LOADER_GLEW" 
        )
endif()

# add include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
