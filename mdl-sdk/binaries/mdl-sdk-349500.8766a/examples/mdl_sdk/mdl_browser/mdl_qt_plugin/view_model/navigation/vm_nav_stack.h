/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief View model class for package navigation.
///        Provides the view models for navigation levels while traversing.
///        Belongs to the NavStack.qml which defines the visual presentation.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_STACK_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_STACK_H

#include <QObject>

class Mdl_browser_tree;
class VM_nav_stack_level_proxy_model;
class VM_nav_package;


class VM_nav_stack : public QObject
{
    Q_OBJECT
public:
    explicit VM_nav_stack(QObject* parent = nullptr, Mdl_browser_tree* browser_tree = nullptr);
    virtual ~VM_nav_stack() = default;

    // called from QML when a module was selected or deselected (nullptr in the latter case)
    // independent of the type of navigation used
    Q_INVOKABLE void set_selected_module(VM_nav_package* package);

    // called from QML to get a initial level to display (first item on the stack)
    Q_INVOKABLE VM_nav_stack_level_proxy_model* create_root_level();

    // called from QML while navigating to a deeper level 
    Q_INVOKABLE VM_nav_stack_level_proxy_model* expand_package(VM_nav_package* package);

    // is called from QML at the end of the navigation to a deeper level or after going back
    Q_INVOKABLE void set_current_level(VM_nav_stack_level_proxy_model* current);


    Q_INVOKABLE void dispose_level(VM_nav_stack_level_proxy_model* level);

signals:
    // signal emitted by 'set_current_level'
    // used to inform the 'selection' (connection is made by the View_model)
    void selected_package_changed(VM_nav_package* p);

    // signal emitted by 'set_selected_module'
    // used to inform the 'selection' (connection is made by the View_model)
    void selected_module_changed(VM_nav_package* m);

public slots:
    // after the filtering changed, we update local counters to eventually hide irrelevant
    // packages and modules (connection is made by the View_model)
    void update_presentation_counters();

private:
    Mdl_browser_tree* m_browser_tree;
    VM_nav_stack_level_proxy_model* m_current_level;
};

#endif
