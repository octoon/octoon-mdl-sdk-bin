/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Main view model class that can be globally accessed from QML


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_VIEW_MODEL_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_VIEW_MODEL_H

#include <QObject>
#include <mi/mdl_sdk.h>
#include "selection/selection_filter.h"
#include "../mdl_browser_settings.h"

struct Mdl_browser_callbacks;
class Mdl_browser_tree;

class VM_nav_stack;
class VM_sel_model;
class VM_sel_proxy_model;

class Mdl_cache;

class View_model : public QObject
{
Q_OBJECT

public:
    explicit View_model() {};
    explicit View_model(mi::neuraylib::INeuray* neuray,
                        mi::neuraylib::ITransaction* transaction,
                        Mdl_browser_callbacks* callbacks,
                        bool cache_rebuild,
                        const char* application_folder);

    virtual ~View_model();

    // navigation component
    Q_PROPERTY(VM_nav_stack* navigation             READ get_navigation
                                                    NOTIFY navigation_changed)
        VM_nav_stack* get_navigation() const { return m_navigation; }

    // selection component
    Q_PROPERTY(VM_sel_proxy_model* selectionModel   READ get_selection_model
                                                    NOTIFY selection_model_changed)
        VM_sel_proxy_model* get_selection_model() const { return m_selection_proxy_model; }

    // persistent application settings
    Q_PROPERTY(Mdl_browser_settings* settings       READ get_settings
                                                    NOTIFY settings_changed)
        Mdl_browser_settings* get_settings() { return &m_settings; }

    // navigation component
    Q_PROPERTY(QString application_folder          READ get_application_folder
               NOTIFY navigation_changed)
        QString get_application_folder() const { return m_application_folder; }

    // header is quite simple at the moment, so we don't add an extra component

    // invoked when search query changed
    Q_INVOKABLE void update_user_filter(const QString& text);

    // invoked when the sorting is changed (button, or when hitting enter in the search field)
    Q_INVOKABLE void set_sort_mode(QString sortBy, bool sortAscending);


    // passing configurations from the application an returning results is currently kept simple

    // called when the user confirms the selection after that, the window is closed
    Q_INVOKABLE void set_result_and_close(const QString& text);

    // called from the entry-function that opened the browser to get the result
    // and also from connecting qml components
    Q_INVOKABLE const QString get_result() const { return m_result; }

    // called when a selection was made
    Q_INVOKABLE void accept();

    // called when the browser was closed without accepting
    Q_INVOKABLE void reject();

signals:
    void navigation_changed();
    void selection_model_changed();
    void settings_changed();

    // signals before and after the search query changed
    void user_filter_is_about_to_change(); // TODO required?
    void user_filter_changed();

    void close_window(); // TODO required?

private:
    mi::base::Handle<mi::neuraylib::INeuray> m_neuray;
    mi::base::Handle<mi::neuraylib::ITransaction> m_transaction;
    Mdl_cache* m_cache;

    Mdl_browser_tree* m_browser_tree;
    VM_nav_stack* m_navigation;
    VM_sel_model* m_selection_model;
    VM_sel_proxy_model* m_selection_proxy_model;
    Selection_filter_composed_search* m_user_filter;

    Mdl_browser_settings m_settings;
    QString m_application_folder;
    QString m_result;
    Mdl_browser_callbacks* m_callbacks;
};

#endif
