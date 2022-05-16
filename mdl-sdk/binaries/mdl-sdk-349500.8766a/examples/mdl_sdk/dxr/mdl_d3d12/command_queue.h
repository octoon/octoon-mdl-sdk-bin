/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/dxr/mdl_d3d12/command_queue.h

#ifndef MDL_D3D12_COMMAND_QUEUE_H
#define MDL_D3D12_COMMAND_QUEUE_H

#include "common.h"
#include "base_application.h"

namespace mi { namespace examples { namespace mdl_d3d12
{
    class Base_application;
    class Fence;

    // --------------------------------------------------------------------------------------------

    class Command_queue
    {

    public:
        explicit Command_queue(Base_application* app, D3D12_COMMAND_LIST_TYPE type);
        virtual ~Command_queue();

        D3DCommandList* get_command_list();
        ID3D12CommandQueue* get_queue() { return m_command_queue.Get(); }
        Fence* get_fence() { return m_fence; }

        UINT64 execute_command_list(D3DCommandList* command_list);

        void flush();

    private:
        struct Allocator_queue_item
        {
            UINT64 handle;
            ID3D12CommandAllocator* allocator;
        };

        ComPtr<ID3D12CommandAllocator> create_command_allocator();
        ComPtr<D3DCommandList> create_command_list(ID3D12CommandAllocator* allocator);

        Base_application* m_app;

        const D3D12_COMMAND_LIST_TYPE m_command_list_type;
        ComPtr<ID3D12CommandQueue> m_command_queue;
        Fence* m_fence;

        std::mutex m_mtx;
        std::vector<ComPtr<ID3D12CommandAllocator>> m_all_command_allocators;
        std::queue<Allocator_queue_item> m_command_allocator_queue;

        std::vector<ComPtr<D3DCommandList>> m_all_command_lists;
        std::queue<D3DCommandList*> m_free_command_list_queue;
    };

    // --------------------------------------------------------------------------------------------

    class Fence
    {
    public:
        explicit Fence(Base_application* app, Command_queue* queue);
        virtual ~Fence();

        UINT64 signal();
        bool is_completed(const UINT64& handle) const;
        bool wait(const UINT64& handle) const;

    private:
        Base_application* m_app;
        std::mutex m_mtx;
        Command_queue* m_command_queue;
        ComPtr<ID3D12Fence> m_fence;
        HANDLE m_fence_event;
        UINT64 m_fence_value;
    };

}}} // mi::examples::mdl_d3d12
#endif
