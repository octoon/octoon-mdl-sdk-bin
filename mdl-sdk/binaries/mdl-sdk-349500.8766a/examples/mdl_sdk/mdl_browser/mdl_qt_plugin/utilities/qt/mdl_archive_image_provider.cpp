/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "mdl_archive_image_provider.h"
#include "utilities/string_helper.h"

Mdl_archive_image_provider::Mdl_archive_image_provider(mi::neuraylib::INeuray* neuray)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    m_mdl_archive_api = neuray->get_api_component<mi::neuraylib::IMdl_archive_api>();
}

QPixmap Mdl_archive_image_provider::requestPixmap(const QString& id, QSize* size,
                                                  const QSize& requestedSize)
{
    // split into archive and file name
    std::string resource_path = id.toUtf8().constData();
    resource_path = String_helper::replace(resource_path, "%5C", "\\");

    // fetch file
    mi::base::Handle<mi::neuraylib::IReader> reader(
        m_mdl_archive_api->get_file(resource_path.c_str()));

    const mi::Sint64 file_size = reader->get_file_size();
    QByteArray buffer(file_size, 0);

    // load the image
    QPixmap image;
    QPixmap result;
    if(reader->read(buffer.data(), file_size) > 0 && image.loadFromData(buffer))
    {
        // resize
        if (requestedSize.isValid())
            result = image.scaled(requestedSize, Qt::KeepAspectRatio);
        else
            result = image;
    }

    // update size information
    *size = result.size();
    return result;
}
