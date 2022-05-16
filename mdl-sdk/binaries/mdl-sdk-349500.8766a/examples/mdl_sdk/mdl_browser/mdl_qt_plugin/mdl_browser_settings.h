/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Persistent settings of this application


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_BROWSER_SETTINGS_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_BROWSER_SETTINGS_H

#include "utilities/application_settings.h"
#include <QObject>
#include <qhash.h>
#include <QVariant>
#include <QMetaType>
#include <string>

class Mdl_browser_settings 
    : public QObject
    , public Application_settings_base
{
    Q_OBJECT

public:

    explicit Mdl_browser_settings(const std::string& auto_save_path = "");
    virtual ~Mdl_browser_settings() = default;


    // the value of the sorting criteria selected last (time the browser was used)
    Q_PROPERTY(QString last_sort_critereon          READ get_last_sort_critereon)
        QString get_last_sort_critereon() const { return last_sort_critereon.get().c_str(); }

    // to order when sorting by relevance
    Q_PROPERTY(bool sort_by_relevance_ascending     READ get_sort_by_relevance_ascending)
        bool get_sort_by_relevance_ascending() const { return sort_by_relevance_ascending; }

    // to order when sorting by name
    Q_PROPERTY(bool sort_by_name_ascending          READ get_sort_by_name_ascending)
        bool get_sort_by_name_ascending() const { return sort_by_name_ascending; }

    // to order when sorting by date
    Q_PROPERTY(bool sort_by_date_ascending          READ get_sort_by_date_ascending)
        bool get_sort_by_date_ascending() const { return sort_by_date_ascending; }

    // the selected view mode, grid or list, selected last (time the browser was used)
    Q_PROPERTY(QString last_view_mode               READ get_last_view_mode
                                                    WRITE set_last_view_mode 
                                                    NOTIFY last_view_mode_changed)
        QString get_last_view_mode() const { return last_view_mode.get().c_str(); }
        void set_last_view_mode(const QString& value);

    // persistent properties
    SettingString last_navigation_package;
    SettingString last_user_query;
    SettingString last_sort_critereon;
    SettingBool sort_by_relevance_ascending;
    SettingBool sort_by_name_ascending;
    SettingBool sort_by_date_ascending;
    SettingString last_view_mode;


signals:
    void last_view_mode_changed();
};

#endif 
