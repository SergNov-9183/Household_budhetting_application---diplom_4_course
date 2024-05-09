import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root

    property bool isDefault: false

    property color fontNormalColor: Style.colors.font
    property color fontDefaultColor: "#000000"

    implicitWidth: label.implicitWidth + 2 * Style.margins.base
    implicitHeight: Math.max(Style.height.base, label.implicitHeight)
    hoverEnabled: enabled
    background: Rectangle {
        border {
            width: Style.width.border
            color: root.isDefault ? Style.colors.focused : Style.expandingButton.borderColor
        }
        radius: Style.radius.base
        color: root.hovered ? Style.expandingButton.hoveredColor : "transparent"
        opacity: root.enabled ? Style.opacity.enabled : Style.opacity.disabled
        HFLabel {
            id: label
            anchors.centerIn: parent
            text: root.text
            color: root.selected || root.hovered ? Style.tabButton.selectedColor : Style.tabButton.normalColor
        }
    }
}
