import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

AbstractButton {
    id: root

    property bool active: false

    implicitHeight: nameEditor.implicitHeight * 1.2
    implicitWidth: rowLayout.implicitWidth
    background: Rectangle {
        border {
            width: Style.width.border
            color: root.active ? Style.expandingButton.borderColor : "transparent"
        }
        radius: Style.radius.base
        color: "transparent"

        RowLayout {
            id: rowLayout
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                rightMargin: Style.margins.base / 2
            }
            spacing: Style.margins.base

            EditLabel {
                id: nameEditor
                Layout.fillWidth: true
                text: root.text
            }

            IconButton {
                visible: root.hovered || root.active
                iconName: "Rename"
                onClicked: nameEditor.startEditing()
            }
        }
    }
}
