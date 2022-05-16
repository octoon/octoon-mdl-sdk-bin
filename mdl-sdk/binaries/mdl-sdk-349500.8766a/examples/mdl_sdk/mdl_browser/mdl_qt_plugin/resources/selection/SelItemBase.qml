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
    width: parent.width
    height: parent.height

    property var infoColor: Material.color(Material.Grey, Material.Shade500)
    property bool isSelected: false
    property string buttonText: "confirm"

    function confirm() {
        confirmed(elementModuleName + "::" +elementName)
    }

    signal confirmed(string value);

    onDoubleClicked: confirm()
}
