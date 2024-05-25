import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root

    property bool active: false
    property alias elide: label.elide
    property bool selectable: true

    implicitHeight: label.implicitHeight * 1.2
    implicitWidth: label.implicitWidth
    background: Rectangle {
        border {
            width: Style.width.border
            color: root.active && root.selectable ? Style.expandingButton.borderColor : "transparent"
        }
        radius: Style.radius.base
        color: root.hovered && root.selectable ? Style.expandingButton.hoveredColor : "transparent"

        HFLabel {
            id: label
            anchors {
                left: parent.left
                leftMargin: Style.margins.base / 2
                right: parent.right
                rightMargin: Style.margins.base / 2
                verticalCenter: parent.verticalCenter
            }
            text: root.text
        }
    }
}
