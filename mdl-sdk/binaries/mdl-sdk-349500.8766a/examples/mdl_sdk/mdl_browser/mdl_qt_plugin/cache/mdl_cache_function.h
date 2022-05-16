/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Implementation of the IMdl_cache_function interface.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_FUNCTION
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_ITEM_FUNCTION

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

class Mdl_cache_function : public Mdl_cache_element<IMdl_cache_function>
{
public:
    explicit Mdl_cache_function() = default;
    virtual ~Mdl_cache_function() = default;

    IMdl_cache_item::Kind get_kind() const override { return CK_FUNCTION; }

    bool update(mi::neuraylib::INeuray* neuray, 
                mi::neuraylib::ITransaction* transaction, 
                const mi::base::IInterface* module) override;

protected:
    typedef Mdl_cache_element<IMdl_cache_function> Base;
};

#endif