/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "../utils" as Utils

SelItemBase {
    id: id_control
    width: 96
    height: 96+30
    spacing: 0

    property int marginHorizontal: 8
    property int marginVertical: 8
    property int buttonWidth: 100
    property int buttonHeight: 40

    Rectangle {
        id: id_background
        anchors.fill: parent
        anchors.leftMargin: marginHorizontal
        anchors.rightMargin: marginHorizontal
        anchors.topMargin: marginVertical
        anchors.bottomMargin: marginVertical

        color: id_control.hovered ? Material.primary : Material.background
        border.width: 0
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 0
            radius: 6.0
            samples: 16
            color: "#80000000"
        }
    }

    Utils.Icon {
        id: id_thumbnail
        anchors.top: id_background.top
        anchors.left: id_background.left
        width: id_background.width
        height: id_background.width

        // full_file_path overrides default one if not empty
        file_path: "../graphics/default_thumbnail_material"
        full_file_path: (elementThumbnail === "") ? "" : 
            ((elementLocatedInArchive ? "image://mdl_archive/" : "file:///") + elementThumbnail)
        color: "transparent" // no tinting
        clickable: false // do not handle clicks
    }

    Rectangle{
        id: id_info_bar
        anchors.left: id_background.left
        anchors.right: id_background.right
        anchors.top: id_thumbnail.bottom
        anchors.bottom: id_background.bottom

        color: Material.background

        Label {
            id: id_title
            anchors.fill: id_info_bar
            anchors.leftMargin: 5
            anchors.rightMargin: 30
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            //color: isSelected ? Material.accent : Material.foreground
            text: (elementDisplayName !== "" ? elementDisplayName : elementName)
            font.pointSize: 9
            elide: "ElideRight"
        }
    }

    Button {
        id: id_submission
        visible: isSelected
        anchors.top: id_control.top
        anchors.topMargin: id_control.marginVertical - 3
        anchors.right: id_control.right
        anchors.rightMargin: id_control.marginHorizontal + 3
        width: buttonWidth
        height: buttonHeight

        text: buttonText
        highlighted: true
        Material.background: Material.accent
        Material.elevation: 5

        onClicked: id_control.confirm()
    }

    Rectangle {
        id: id_selectionBar
        visible: isSelected
        anchors.top: id_info_bar.top
        anchors.bottom: id_background.bottom
        anchors.right: id_background.right

        color: Material.accent
        width: 3
    }

    Utils.Icon {
        id: id_icon
        anchors.verticalCenter: id_info_bar.verticalCenter
        anchors.right: id_info_bar.right
        anchors.rightMargin: 8
        width: 16
        height: 16

        file_path: "../graphics/info_icon"
        color: hovered 
            ? Material.foreground
            : infoColor

        onHoveredChanged: {
            if(hovered) id_toolTip.show();
            else id_toolTip.hide();
        }

        Utils.ToolTipExtended {
            id: id_toolTip
            delay: 500
            visible: id_icon.hovered
            text:  elementModuleHint
            maximumWidth: 400
            font.pointSize: 10
        }
    }
}
