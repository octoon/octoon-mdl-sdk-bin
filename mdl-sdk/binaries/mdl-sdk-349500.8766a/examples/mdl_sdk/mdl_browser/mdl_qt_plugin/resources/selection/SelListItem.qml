/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Window 2.3

import "../utils" as Utils

SelItemBase {
    id: id_control
    width: parent.width
    height: Math.max(targetHeight, id_infos.implicitHeight + marginVertical * 2) + ( marginVertical + borderWidth ) * 2
    spacing: 0

    property int targetHeight: 96
    property int marginHorizontal: 5
    property int marginVertical: 3
    property int borderWidth: 0
    property int buttonWidth: 100
    property int buttonHeight: 40


    Rectangle {
        id: id_background
        anchors.fill: parent
        anchors.margins: 3
        color: id_control.hovered ? Material.primary : Material.background
        border.color: id_control.hovered ? Material.primary : Material.background
        border.width: borderWidth

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
        anchors.margins: id_background.border.width
        width: targetHeight
        height: targetHeight

        // full_file_path overrides default one if not empty
        file_path: "../graphics/default_thumbnail_material"
        full_file_path: (elementThumbnail === "") ? "" : 
            ((elementLocatedInArchive ? "image://mdl_archive/" : "file:///") + elementThumbnail)
        color: "transparent" // no tinting
        clickable: false // do not handle clicks
    }

    Item {
        id: id_infos
        anchors.fill: id_background
        anchors.topMargin: id_control.marginVertical
        anchors.bottomMargin: id_control.marginVertical
        anchors.leftMargin: id_thumbnail.width + (id_control.borderWidth + id_control.marginHorizontal) * 2
        anchors.rightMargin: buttonWidth + id_submission.anchors.rightMargin + id_control.borderWidth + id_control.marginHorizontal

        Item {
            id: id_title
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top
            height: id_title_name.implicitHeight

            Label {
                id: id_title_name
                anchors.left: parent.left
                width: Math.min(implicitWidth, id_infos.width)
                color: Material.foreground
                text: (elementDisplayName !== "" ? elementDisplayName : elementName)
                font.pointSize: 11
                elide: "ElideRight"
            }

            Label {
                id: id_title_by
                visible: (elementAuthor !== "")
                anchors.left: id_title_name.right
                anchors.bottom: id_title_name.bottom
                anchors.right: parent.right
                color: id_control.infoColor
                text: " by " +  elementAuthor
                font.pointSize: 10
                elide: "ElideRight"
            }
        }

        Text {
            id: id_description
            visible: (elementDescription !== "" || elementKeywords !== "")

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: id_title.bottom
            anchors.topMargin: 5
            anchors.bottom: parent.bottom

            color: id_control.infoColor
            text: ((elementKeywords !== "") ? ("Keywords: [" + elementKeywords + "]<br>") : "") + elementDescription
            wrapMode: "WordWrap"
            elide: "ElideRight"
            font.pointSize: 10
            maximumLineCount: 3
        }
    }

    Rectangle {
        id: id_selectionBar
        visible: isSelected
        anchors.top: id_background.top
        anchors.bottom: id_background.bottom
        anchors.right: id_background.right

        color: Material.accent
        width: 3
    }

    Button {
        id: id_submission
        visible: isSelected
        anchors.top: id_background.top
        anchors.topMargin: 5
        anchors.right: id_background.right
        anchors.rightMargin: 15
        width: buttonWidth
        height: buttonHeight

        text: buttonText
        highlighted: true
        Material.background: Material.accent
        Material.elevation: 0

        onClicked: id_control.confirm()
    }

    Label {
        id: id_modification_date
        anchors.bottom: id_background.bottom
        anchors.bottomMargin: 3
        anchors.right: id_background.right
        anchors.rightMargin: 45
        anchors.left: id_thumbnail.right
        anchors.leftMargin: 5

        color: id_control.infoColor
        text: "last modified: " + elementModification
        font.pointSize: 8
        horizontalAlignment: Text.AlignRight
        elide: "ElideLeft"
    }

    Utils.Icon {
        id: id_icon
        anchors.bottom: id_background.bottom
        anchors.bottomMargin: 5
        anchors.right: id_background.right
        anchors.rightMargin: 15
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
