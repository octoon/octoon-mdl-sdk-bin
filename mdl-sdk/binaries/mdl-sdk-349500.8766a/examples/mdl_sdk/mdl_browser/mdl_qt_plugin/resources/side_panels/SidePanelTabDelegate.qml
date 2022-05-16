/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

 Item{
    id: id_control
    width: id_button.implicitHeight
    height: id_button.implicitWidth

    property string text: "Navigation"
    property bool isCurrent: false

    signal clicked
    //clip: true

    ItemDelegate {
        id: id_button
        anchors.top: parent.top
        implicitWidth: id_label.implicitWidth + 20
        implicitHeight: id_label.implicitHeight + 8
        padding: 0
        z:1

        transform: [
            Rotation { origin.x: 0; origin.y: 0; angle: 90},
            Translate { x: id_button.implicitHeight }
        ]

        Rectangle {
            id: id_background
            anchors.fill: parent
            color: Material.background
        }

        InnerShadow {
            visible: !id_control.isCurrent
            anchors.fill: id_background
            horizontalOffset: 0
            verticalOffset: -3
            radius: 12.0
            samples: 16
            color: "#80000000"
            source: id_background
        }

        Rectangle {
            visible: id_control.isCurrent
            color: Material.accent
            anchors.top: id_button.top
            width: id_button.width
            height: 3
        }

        Label {
            id: id_label
            anchors.bottom: id_button.bottom
            anchors.bottomMargin: 3
            anchors.horizontalCenter: id_button.horizontalCenter
            text: id_control.text.toUpperCase()
            font.pointSize: 10
        }

        onClicked: id_control.clicked()
    }
}
