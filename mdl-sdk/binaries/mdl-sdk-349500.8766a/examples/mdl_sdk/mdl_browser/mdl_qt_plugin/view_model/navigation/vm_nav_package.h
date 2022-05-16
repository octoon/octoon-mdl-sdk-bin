/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief View model class to represent a package or module in the navigation panel.
///        Belongs to the NavPackage.qml which defines the visual presentation.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_PACKAG_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_VM_NAV_PACKAG_H

#include <QtCore/QObject>
#include <QtCore/qhash.h>
#include <QtCore/QVariant>


class Mdl_browser_node;


class VM_nav_package : public QObject
{
    Q_OBJECT

public:
    enum NodeRole
    {
        NameRole = Qt::UserRole,
        IsModuleRole,
        IsShadowing,
        ShadowHintRole,
        ShortModeRole,
        ShortModeSavingRole,
        PresentationCountRole,
    };
    Q_ENUM(NodeRole)


    explicit VM_nav_package(QObject* parent = nullptr, Mdl_browser_node* node = nullptr);
    virtual ~VM_nav_package() = default;


    // name of the package or module
    Q_PROPERTY(QString packageName          READ get_name)
        QString get_name() const;

    // indicated the type (package or module)
    Q_PROPERTY(bool packageIsModule         READ get_is_module)
        bool get_is_module() const;

    // in case this is a module, does this module has shadows
    Q_PROPERTY(bool moduleIsShadowing       READ get_is_shadowing)
        bool get_is_shadowing() const;

    // tool tip text in case this module has shadows
    Q_PROPERTY(QString moduleShadowHint     READ get_shadow_hint)
        QString get_shadow_hint() const;

    // used for breadcrumbs: indicate if the name is reduced to the first character
    Q_PROPERTY(bool packageShortMode        READ get_short_mode 
                                            WRITE set_short_mode 
                                            NOTIFY short_mode_changed)
        bool get_short_mode() const { return m_shortMode; }
        void set_short_mode(bool value);

    // used for breadcrumbs: width that can be saved when reducing to the first character
    Q_PROPERTY(float packageShortModeSaving READ get_short_mode_saving 
                                            WRITE set_short_mode_saving 
                                            NOTIFY short_mode_saving_changed)
        float get_short_mode_saving() const { return m_shortModeSaving; }
        void set_short_mode_saving(float value);

    // the number of selectable elements in the selection panel that are rooted under this node
    Q_PROPERTY(int packagePresentationCount READ get_presentation_count 
                                            NOTIFY presentation_count_changed)
        int get_presentation_count() const;

       
    // get and set data from QML
    QVariant data(int role) const;
    bool setData(const QVariant& value, int role);
    static QHash<int, QByteArray> roleNames();

    // data object behind this view model
    Mdl_browser_node* get_browser_node() { return m_node; }
    const Mdl_browser_node* get_browser_node() const { return m_node; }

    // called (indirectly) from the model proxy after the set of selectable elements changed
    void update_presentation_counter() const { emit presentation_count_changed(); }
                                              
signals:
    void short_mode_changed();
    void short_mode_saving_changed();
    void presentation_count_changed() const;

private:
    Mdl_browser_node* m_node;
    bool m_shortMode;
    float m_shortModeSaving;
};

#endif
