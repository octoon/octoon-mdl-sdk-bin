/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "../utils" as Utils

ItemDelegate {
    id: id_control
    implicitHeight: id_icon.implicitHeight
    implicitWidth: id_icon.implicitWidth

    signal selected();

    property bool isSelected: true
    property string name: "Name"
    property string iconFilePath: ""

    Utils.Icon {
        id: id_icon
        anchors.centerIn: parent
        width: 16
        height: 16
        file_path: id_control.iconFilePath
        color: isSelected
            ? Material.accent
            : Material.foreground
        clickable: false
    }

    onClicked: selected()
}
