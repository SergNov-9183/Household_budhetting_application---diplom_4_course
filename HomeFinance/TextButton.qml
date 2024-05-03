import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root

    property bool isDefault: false

    property color backgroundNormal: "#2D2D2D"
    property color backgroundNormalHovered: "#323232"
    property color backgroundDefault: "#40BDFF"
    property color backgroundDefaultHovered: "#3CADE8"
    property color fontNormalColor: Style.colors.font
    property color fontDefaultColor: "#000000"

    implicitWidth: label.implicitWidth + 20
    implicitHeight: Math.max(Style.height.base, label.implicitHeight)
    hoverEnabled: enabled
    background: Rectangle {
        color: root.isDefault
               ? root.hovered ? root.backgroundDefaultHovered : root.backgroundDefault
               : root.hovered ? root.backgroundNormalHovered : root.backgroundNormal
        radius: Style.radius.base
        opacity: root.enabled ? Style.opacity.enabled : Style.opacity.disabled
        HFLabel {
            id: label
            anchors.centerIn: parent
            text: root.text
            color: root.isDefault ? root.fontDefaultColor : root.fontNormalColor
        }
    }
}
