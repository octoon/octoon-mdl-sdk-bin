/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "vm_nav_stack_level_proxy_model.h"
#include "vm_nav_stack_level_model.h"

VM_nav_stack_level_proxy_model::VM_nav_stack_level_proxy_model(QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(nullptr)
{
}

void VM_nav_stack_level_proxy_model::setSourceModel(QAbstractItemModel* source_model)
{
    m_model = dynamic_cast<VM_nav_stack_level_model*>(source_model);
    QSortFilterProxyModel::setSourceModel(source_model);

    // encode the sorter index as column to sort
    sort(0, Qt::AscendingOrder);
}

Q_INVOKABLE VM_nav_package* VM_nav_stack_level_proxy_model::get_package(int index) const
{
    const QModelIndex proxy_index = QSortFilterProxyModel::index(index, 0); // proxy index
    const QModelIndex source_index = mapToSource(proxy_index); // source index
    return m_model->get_package(source_index.row());
}

VM_nav_package* VM_nav_stack_level_proxy_model::get_current_package()
{
    return m_model->get_current_package();
}
bool VM_nav_stack_level_proxy_model::filterAcceptsRow(int source_row,
                                                      const QModelIndex &source_parent) const
{
    // only show packages and modules with selectable elements
    return m_model->get_package(source_row)->get_presentation_count() > 0;
}
