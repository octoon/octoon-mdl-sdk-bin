/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Flickable {
    id: id_control
    property SidePanel leftPanel
    property SidePanel rightPanel

    property alias main : id_content.children
    interactive: false

    // main content area
    Item {
        id: id_content
        anchors.fill: parent
    }

    Component.onCompleted: {
        if(leftPanel) {
            leftPanel.onPositionChanged.connect(updateContentSize);
            leftPanel.onWidthChanged.connect(updateContentSize);
        }
        if(rightPanel) {
            rightPanel.onPositionChanged.connect(updateContentSize);
            rightPanel.onWidthChanged.connect(updateContentSize);
        }
        updateContentSize();
    }

    function updateContentSize()
    {
        if(leftPanel)
            id_content.anchors.leftMargin = Math.max(leftPanel.width * leftPanel.position, leftPanel.labelWidth);
        if(rightPanel)
            id_content.anchors.rightMargin = Math.max(rightMargin.width * rightMargin.position, rightMargin.labelWidth);
    }
}
