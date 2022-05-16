/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Implementation of the IMdl_cache_module interface.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_MODULE_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_MODULE_H

#include "imdl_cache.h"
#include "mdl_cache_impl.h"

namespace mi
{
    namespace base
    {
        class IInterface;
    }

    namespace neuraylib
    {
        class INeuray;
        class ITransaction;
    }
}

class Mdl_cache_module : public Mdl_cache_node<IMdl_cache_module>
{
public:
    explicit Mdl_cache_module() = default;
    virtual ~Mdl_cache_module() = default;

    IMdl_cache_item::Kind get_kind() const override { return CK_MODULE; }

    bool update(mi::neuraylib::INeuray* neuray, 
                mi::neuraylib::ITransaction* transaction, 
                const mi::base::IInterface* node) override;

    bool get_located_in_archive() const override;

protected:
    typedef Mdl_cache_node<IMdl_cache_module> Base;

private:
    void set_located_in_archive(bool value);
    const char* get_file_path() const;
    void set_file_path(const char* search_path);
    mutable bool m_located_in_archive;
};

#endif