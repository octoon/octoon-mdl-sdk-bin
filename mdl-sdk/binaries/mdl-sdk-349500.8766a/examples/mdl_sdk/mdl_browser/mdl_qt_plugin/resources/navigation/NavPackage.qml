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
    //width: parent.width
    height: 40

    property bool showPresentationCounter: true
    property bool showSepBar: true

    property bool isSelected: true

    padding: 0

    contentItem: Item {
        anchors.fill: parent

        Rectangle {
            id: id_sepBar
            visible: showSepBar
            anchors.top: parent.top
            anchors.topMargin: -1
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: Material.primary
        }

        Utils.Icon {
            id: id_icon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: 16
            height: 16

            file_path: packageIsModule
                ? "../graphics/module_icon"
                : "../graphics/package_icon"
            color: isSelected
                ? Material.accent
                : Material.foreground

            onHoveredChanged: {
                if(!moduleIsShadowing) return;
                if(hovered) id_toolTip.show();
                else id_toolTip.hide();
            }

            Utils.ToolTipExtended {
                id: id_toolTip
                delay: 500
                text:  moduleShadowHint
                maximumWidth: 450
                font.pointSize: 10
            }
        }

        // shadow !
        Label {
            id: id_shadowing
            color: "#FF6644"
            visible: moduleIsShadowing
            anchors.bottom: id_icon.bottom
            anchors.bottomMargin: 3
            anchors.left: id_icon.left
            anchors.leftMargin: 9
            text: "!"
            font.pointSize: 14
            font.bold: true
        }

        Label {
            id: id_name
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: id_icon.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: id_selectionBar.width + 5
            text: packageName
            font.pointSize: 10
            font.bold: true
            elide: Text.ElideRight
        }

        Label {
            visible: id_control.showPresentationCounter
            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 10
            text: packagePresentationCount
            font.pointSize: 7.5
            horizontalAlignment: Text.AlignRight
        }

        Rectangle {
            id: id_selectionBar
            visible: isSelected
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            color: Material.accent
            width: 3
        }
    }
}
