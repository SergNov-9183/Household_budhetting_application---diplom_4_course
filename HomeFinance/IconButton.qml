import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root

    property bool hideIconOnDisable: false
    property string iconName

    implicitWidth: Style.size.icon
    implicitHeight: Style.size.icon
    background: Item {
        Image {
            anchors.centerIn: parent
            visible: root.enabled || !root.hideIconOnDisable
            opacity: root.enabled ? Style.opacity.enabled : Style.opacity.disabled
            source: "qrc:/Images/" + iconName + (root.enabled && root.hovered ? "-Hover" : "") + ".svg"
        }
    }
}
