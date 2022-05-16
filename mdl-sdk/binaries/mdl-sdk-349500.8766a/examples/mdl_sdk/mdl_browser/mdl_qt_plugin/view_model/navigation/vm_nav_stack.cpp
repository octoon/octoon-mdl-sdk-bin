/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "vm_nav_stack.h"
#include "vm_nav_stack_level_model.h"
#include "vm_nav_package.h"

#include <mi/mdl_sdk.h>
#include "../../cache/mdl_cache_package.h"
#include "../../mdl_browser_node.h"
#include "vm_nav_stack_level_proxy_model.h"


VM_nav_stack::VM_nav_stack(QObject* parent, Mdl_browser_tree* browser_tree) 
    : QObject(parent)
    , m_browser_tree(browser_tree)
{
}

Q_INVOKABLE VM_nav_stack_level_proxy_model* VM_nav_stack::create_root_level()
{
    auto proxy = new VM_nav_stack_level_proxy_model(this);
    proxy->setSourceModel(new VM_nav_stack_level_model(this, m_browser_tree->get_root()));
    return proxy;
}

Q_INVOKABLE VM_nav_stack_level_proxy_model* VM_nav_stack::expand_package(VM_nav_package* package)
{
    auto proxy = new VM_nav_stack_level_proxy_model(this);
    proxy->setSourceModel(new VM_nav_stack_level_model(this, package->get_browser_node()));
    return proxy;
}

Q_INVOKABLE void VM_nav_stack::dispose_level(VM_nav_stack_level_proxy_model* level)
{
    // because of the passed this pointer in the methods above,
    // we don't need to free levels
    // to save memory in case of heavy use, we should do it manually
}

Q_INVOKABLE void VM_nav_stack::set_current_level(VM_nav_stack_level_proxy_model* current)
{
    if (!current)
    {
        std::cerr << "[VM_nav_stack] set_current: passing a 'nullptr' is not allowed.\n";
        return;
    }

    m_current_level = current;

    auto model = dynamic_cast<VM_nav_stack_level_model*>(m_current_level->sourceModel());

    //std::cout << "[VM_nav_stack] set_current_level: current level is: " 
    //          << model->get_current_package()->get_name().toUtf8().data() << "\n";
    emit selected_package_changed(model->get_current_package());
}

Q_INVOKABLE void VM_nav_stack::set_selected_module(VM_nav_package* m)
{
    if (!m)
    {
        //std::cout << "[VM_nav_stack] set_selected_module: to none\n";
        emit selected_module_changed(nullptr);
        return;
    }
    
    //std::cout << "[VM_nav_stack] set_selected_module: current module is: " 
    //          << m->get_name().toUtf8().data() << "\n";

    emit selected_module_changed(m);
}

Q_INVOKABLE void VM_nav_stack::update_presentation_counters()
{
    if (!m_current_level) return;

    auto model = dynamic_cast<VM_nav_stack_level_model*>(m_current_level->sourceModel());
    model->update_presentation_counters();
    m_current_level->invalidate();
}
