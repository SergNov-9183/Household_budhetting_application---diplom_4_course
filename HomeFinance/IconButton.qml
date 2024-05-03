import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root

    property bool selected: false
    property bool hideIconOnDisable: false
    property string normalIcon
    property string hoveredIcon
    property string selectedIcon

    implicitWidth: Style.size.icon
    implicitHeight: Style.size.icon
    background: Item {
        Image {
            anchors.centerIn: parent
            visible: root.enabled || !root.hideIconOnDisable
            opacity: root.enabled ? Style.opacity.enabled : Style.opacity.disabled
            source: "qrc:/Images/" + (root.enabled
                                      ? root.hovered
                                        ? root.hoveredIcon
                                        : root.selected
                                          ? root.selectedIcon
                                          : root.normalIcon
                                      : root.normalIcon) + ".svg"
        }
    }
}
