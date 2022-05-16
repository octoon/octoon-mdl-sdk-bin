/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "mdl_browser_settings.h"
#include "utilities/application_settings_serializer_xml.h"

Mdl_browser_settings::Mdl_browser_settings(const std::string& auto_save_path)
    : Application_settings_base(
        new Application_settings_serializer_xml(), // change for other serialization interface
        auto_save_path  // if true, setting a value will update the file
)
, last_navigation_package(*this, "LastNavigationPackage", "::")
, last_user_query(*this, "LastUserQuery", "")
, last_sort_critereon(*this, "LastSortCriterion", "Relevance")
, sort_by_relevance_ascending(*this, "SortByRelevanceAscending", false)
, sort_by_name_ascending(*this, "SortByNameAscending", true)
, sort_by_date_ascending(*this, "SortByDateAscending", false)
, last_view_mode(*this, "LastViewMode", "List")
{
}

void Mdl_browser_settings::set_last_view_mode(const QString& value)
{
    last_view_mode = value.toUtf8().constData();
    emit last_view_mode_changed();
}
