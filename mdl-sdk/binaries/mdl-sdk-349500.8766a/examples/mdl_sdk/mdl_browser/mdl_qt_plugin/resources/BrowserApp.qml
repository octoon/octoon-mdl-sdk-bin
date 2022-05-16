/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import MdlQtPlugin 1.0 as Mdl

ApplicationWindow {
    visible: true
    width: 1280
    height: 720

    title: "MDL Material Browser"

    property bool keep_previous_select: false   // keeps the selected object selected 
                                                // when opening the dialog the next time

    QtObject {
        id: id_private
        property bool accepted: false
    }

    Mdl.BrowserMain {
        id: id_main
        anchors.fill: parent

        onAccepted: {
            id_private.accepted = true;
            close()
        }

        onRejected: {
            id_private.accepted = false;
            close()
        }
    }

    onVisibilityChanged: {

        // reset to defaults when the window is shown
        if(visible) {   
            id_private.accepted = false;

            if(!keep_previous_select)
                id_main.reset();
        }

        // handle closing of the dialog as reject
        else {
            if(!id_private.accepted)
                id_main.reject();
        }
    }
}