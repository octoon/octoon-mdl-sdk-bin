/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.3
import MdlQtPlugin 1.0 as Mdl

Dialog {
    id: id_control
    property string result: ""                  // contains the selected material, function, ...
                                                // or an empty string

    property bool keep_previous_select: false   // keeps the selected object selected 
                                                // when opening the dialog the next time

    width: 1280                                 // default window width
    height: 720                                 // default window height
    visible: false                              // initially hidden, call .open() to show it
    title: "MDL Material Browser Dialog"        // window title that can be overridden 

    QtObject {
        id: id_private
        property bool accepted;
    }

    contentItem: Mdl.BrowserMain {
        id: id_main

        onAccepted: {
            id_private.accepted = true;
            id_control.result = id_main.result;
            id_control.accept();
        }

        onRejected: {
            id_private.accepted = false;
            id_control.result = "";
            id_control.reject();
        }
    }

    onVisibilityChanged: {

        // reset to defaults when the window is shown
        if(visible) {   
            id_private.accepted = false;
            id_control.result = "";

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
