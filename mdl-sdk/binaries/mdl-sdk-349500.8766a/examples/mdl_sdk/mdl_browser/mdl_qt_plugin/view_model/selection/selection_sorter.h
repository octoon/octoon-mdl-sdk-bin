/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Classes for sorting selectable elements, e.g., materials, functions, ...


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_SELECTION_SORTER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_SELECTION_SORTER_H

#include "vm_sel_element.h"


class Index_cache_elements;
class VM_nav_package;

// base class to enable the sorting of the presented selectable elements
class Selection_sorter_base
{
public:
    explicit Selection_sorter_base(const std::string& name, bool ascending = false);
    virtual ~Selection_sorter_base() = default;

    // implementation of the actual comparison
    virtual float compare(const VM_sel_element* left, const VM_sel_element* right) const = 0;

    // name of the criteria, used for identification (UI and Settings)
    const std::string get_name() { return m_name; }

    // oder of sorting
    bool get_ascending() const { return m_ascending; }
    void set_ascending(bool value) { m_ascending = value; }

private:
    const std::string m_name;
    bool m_ascending;

};


// sort by search result ranking
class Selection_sorter_ranking 
    : public Selection_sorter_base
{
public:
    explicit Selection_sorter_ranking(const std::string& name, bool ascending = false)
        : Selection_sorter_base(name, ascending) { }
    virtual ~Selection_sorter_ranking() = default;

    float compare(const VM_sel_element* left, const VM_sel_element* right) const override;
};

// sort by name of the selectable element
class Selection_sorter_name 
    : public Selection_sorter_base
{
public:
    explicit Selection_sorter_name(const std::string& name, bool ascending = false)
        : Selection_sorter_base(name, ascending) { }
    virtual ~Selection_sorter_name() = default;

    float compare(const VM_sel_element* left, const VM_sel_element* right) const override;
};

// sort by date of last change
class Selection_sorter_date 
    : public Selection_sorter_base
{
public:
    explicit Selection_sorter_date(const std::string& name, bool ascending = false)
        : Selection_sorter_base(name, ascending) { }
    virtual ~Selection_sorter_date() = default;

    float compare(const VM_sel_element* left, const VM_sel_element* right) const override;
};


#endif 
