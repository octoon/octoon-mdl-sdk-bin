/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

ItemDelegate {
    id: id_sort_name
    height: 20 // parent.height
    implicitWidth: id_name_label.implicitWidth + 15

    property bool isSelected: true
    property bool ascending: true
    property string name: "Name"

    signal selected();

    Label {
        id: id_name_label
        anchors.verticalCenter: parent.verticalCenter
        text: parent.name
        font.pointSize: 10
        color: (isSelected ? Material.accent : Material.foreground)
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -1
        anchors.left: id_name_label.right
        anchors.leftMargin: 5
        text: parent.ascending ? "↓" : "↑"
        font.pointSize: 10
        color: (isSelected ? Material.accent : Material.foreground)
    }

    onClicked: {
        // if this is currently selected, we change the order
        if(isSelected)
        {
            ascending = !ascending;
            selected();
        }
        // otherwise, we switch without changing the order
        else
        {
            selected();
        }
    }
}
