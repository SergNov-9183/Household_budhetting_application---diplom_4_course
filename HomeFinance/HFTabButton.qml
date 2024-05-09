import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

AbstractButton {
    id: root

    property alias image: imageIcon.source
    property bool selected: true

    implicitHeight: Style.height.base
    implicitWidth: imageIcon.width  + label.implicitWidth + Style.margins.base

    background: Item {
        RowLayout {
            id: rowLayout
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
            }
            spacing: Style.margins.base

            Image { id: imageIcon }

            HFLabel {
                id: label
                Layout.fillWidth: true
                text: root.text
                color: root.selected || root.hovered ? Style.tabButton.selectedColor : Style.tabButton.normalColor
            }
        }

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: 2
            radius: height/2
            color: Style.colors.focused
            visible: root.selected
        }
    }
}
