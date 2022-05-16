/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief qt image provided that allows to load images thar are embedded in an archive


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_ARCHIVE_IMAGE_PROVIDER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_ARCHIVE_IMAGE_PROVIDER_H


#include <QQuickImageProvider>
#include <QtGui/QImage>

#include "example_shared.h"

namespace mi
{
    namespace neuraylib
    {
        class INeuray;
        class IMdl_archive_api;
    }
}


class Mdl_archive_image_provider : public QQuickImageProvider
{
public:
    explicit Mdl_archive_image_provider(mi::neuraylib::INeuray* neuray);
    virtual ~Mdl_archive_image_provider() = default;

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    mi::base::Handle<mi::neuraylib::IMdl_archive_api> m_mdl_archive_api;
};

#endif
