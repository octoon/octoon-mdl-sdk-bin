/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Implementation of the IMdl_cache_package interface.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_PACKAGE_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_PACKAGE_H

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

class Mdl_cache_package : public Mdl_cache_node<IMdl_cache_package>
{
public:
    explicit Mdl_cache_package() = default;
    virtual ~Mdl_cache_package() = default;

    IMdl_cache_item::Kind get_kind() const override { return CK_PACKAGE; }

    bool update(mi::neuraylib::INeuray* neuray, 
                mi::neuraylib::ITransaction* transaction, 
                const mi::base::IInterface* node) override;

protected:
    typedef Mdl_cache_node<IMdl_cache_package> Base;
};

#endif