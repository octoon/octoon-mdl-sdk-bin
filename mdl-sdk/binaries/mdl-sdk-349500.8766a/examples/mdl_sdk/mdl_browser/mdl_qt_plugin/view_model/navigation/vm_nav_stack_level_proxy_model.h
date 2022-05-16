/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief View model proxy class for package navigation.
///        This class filters the data model of the VM_nav_stack_level_model.
///        It stores no own data but influences the visual presentation.
///        Belongs to the ListView in NavStack.qml which defines the visual presentation.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_STACK_LEVEL_PROXY_MODEL_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_STACK_LEVEL_PROXY_MODEL_H

#include <QObject>
#include <QSortFilterProxyModel>


class VM_nav_package;
class VM_nav_stack_level_model;

class VM_nav_stack_level_proxy_model : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit VM_nav_stack_level_proxy_model(QObject* parent = nullptr);
    virtual ~VM_nav_stack_level_proxy_model() = default;

    //  item of this level (the VM_nav_package can represent both, packages and modules)
    Q_PROPERTY(VM_nav_package* currentPackage   READ get_current_package)
        VM_nav_package* get_current_package();

    // get an item of this level (the VM_nav_package can represent both, packages and modules)
    Q_INVOKABLE VM_nav_package* get_package(int index) const;

    // sets the underlaying VM_nav_stack_level_model
    // called by the VM_nav_stack
    void setSourceModel(QAbstractItemModel* source_model) override;

protected:
    // implements the filtering of packages and modules
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    VM_nav_stack_level_model* m_model;
};

#endif
