/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Plugin registration

#ifndef MDL_SDK_EXAMPLES_MDL_PLUGIN_H 
#define MDL_SDK_EXAMPLES_MDL_PLUGIN_H

#include <QtQml/QQmlExtensionPlugin>
#include "include/mdl_qt_plugin.h"

class Mdl_archive_image_provider;
class View_model;

class QPluginLoader;



class Mdl_qt_plugin : public QQmlExtensionPlugin, public Mdl_qt_plugin_interface
{
    Q_OBJECT
    Q_INTERFACES(Mdl_qt_plugin_interface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0" FILE "metadata.json")

public:
    explicit Mdl_qt_plugin();
    virtual ~Mdl_qt_plugin() = default;
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri) override;
    bool set_context(
        QQmlApplicationEngine* engine,
        Mdl_qt_plugin_context* context) final override;
    void show_select_material_dialog(
        Mdl_qt_plugin_context* context,
        Mdl_qt_plguin_browser_handle* out_handle) final override;
    void unload() final override;

protected:
    bool initialize(QPluginLoader* loader) final override;

private:
    QQmlApplicationEngine* m_engine;
    View_model* m_view_model;
    QPluginLoader* m_loader;
};
#endif
