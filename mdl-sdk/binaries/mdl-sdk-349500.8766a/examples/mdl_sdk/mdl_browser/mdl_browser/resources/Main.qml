/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

// import the MDL module and use the Mdl namespace for a more explicit usage
import MdlQtPlugin 1.0 as Mdl


ApplicationWindow {
    id: id_window
    visible: true
    width: 640
    height: 360

    title: "MDL Application"
    
    
    // the mdl browser dialog, which is initially hidden
    Mdl.BrowserDialog {
        id: id_select_material_dialog

        modality: Qt.NonModal
        title: "MDL Material Browser Dialog"
        keep_previous_select: true

        onAccepted: {
            id_last_selection.text = result;
        }

        onRejected: {
            id_last_selection.text = "selection aborted";
        }

    }

    // background
    Rectangle {
        anchors.fill: parent

        Button {
            id: id_open_dialog_button
            text: "select material"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: id_select_material_dialog.open()
        }

        Label {
            id: id_last_selection
            anchors.top: id_open_dialog_button.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: "nothing selected yet"

        }
    }
}
