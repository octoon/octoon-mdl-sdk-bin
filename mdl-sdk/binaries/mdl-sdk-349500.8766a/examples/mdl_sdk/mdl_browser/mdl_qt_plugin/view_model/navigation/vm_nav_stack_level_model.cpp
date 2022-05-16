/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "vm_nav_stack_level_model.h"
#include <iostream>
#include "../../mdl_browser_node.h"


// Compare two variants.
bool sort_nav_packages(const VM_nav_package* v1, const VM_nav_package* v2)
{
    // packages need to go up
    bool v1_is_module = v1->get_is_module();
    bool v2_is_module = v2->get_is_module();
    if (!v1_is_module &&  v2_is_module) return true;
    if ( v1_is_module && !v2_is_module) return false;

    // sort by name in the other cases
    return v1->get_name().toLower() < v2->get_name().toLower();
}

VM_nav_stack_level_model::VM_nav_stack_level_model(QObject* parent, Mdl_browser_node* node) 
    : QAbstractListModel(parent)
    , m_current_node(node)
{
    m_current_package = new VM_nav_package(this, node);

    // should not happen due to application logic
    if (node->get_is_module())
    {
        std::cerr << "[VM_nav_stack_level] Constructor: " 
                  <<" it is not allowed to create new levels for modules " 
                  << "(as they do not have children): " 
                  << node->get_entity_name() << ".\n";
        return;
    }

    // create the list of child elements for this node that are presented on the UI
    for (const auto& kv : m_current_node->get_children())
        m_child_packages.append(new VM_nav_package(this, kv.second));

    // sort elements using a static order, no dynamic reordering by the user here
    qSort(m_child_packages.begin(), m_child_packages.end(), sort_nav_packages);
}

VM_nav_stack_level_model::~VM_nav_stack_level_model()
{
    m_child_packages.clear();
    if(m_current_package) 
        delete m_current_package;
}

int VM_nav_stack_level_model::rowCount(const QModelIndex&) const
{
    return m_child_packages.count();
}

QVariant VM_nav_stack_level_model::data(const QModelIndex& index, int role) const
{
    if (index.row() < rowCount())
        return m_child_packages[index.row()]->data(role);
    return QVariant();
}

bool VM_nav_stack_level_model::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role < Qt::UserRole)
        return QAbstractListModel::setData(index, value, role);

    if (index.isValid())
    {
        if (m_child_packages[index.row()]->setData(value, role))
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Q_INVOKABLE VM_nav_package* VM_nav_stack_level_model::get_package(int idx) const
{
    return m_child_packages[idx];
}

void VM_nav_stack_level_model::update_presentation_counters()
{
    for (auto& child : m_child_packages)
        child->update_presentation_counter();

    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
}
