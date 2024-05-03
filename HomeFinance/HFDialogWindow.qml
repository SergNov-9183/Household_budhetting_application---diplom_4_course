import QtQuick
import Qt5Compat.GraphicalEffects

import HomeFinance

Item {
    id: root

    default property alias contents: placeholder.data
    property alias okText: okButton.text
    property alias cancelText: cancelButton.text
    property alias okEnabled: okButton.enabled

    signal accepted()

    function show() { visible = true }
    function hide() { visible = false }

    anchors.centerIn: parent
    height: 200
    width: 400
    visible: false

    Item {
        id: backgroundGradient
        anchors.fill: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            anchors.fill: backgroundGradient
            source: gradientFill
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: Style.radius.window
            }
        }

        LinearGradient {
            id: gradientFill
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#1C2125" }
                GradientStop { position: 1.0; color: "#21201A" }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        border {
            color: "#0063B1"
            width: Style.width.border
        }
        color: "transparent"
        radius: Style.radius.window

        FrameItem {
            id: placeholder
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
                bottom: line.top
                margins: Style.margins.base
            }
        }

        Rectangle {
            id: line
            anchors {
                left: parent.left
                right: parent.right
                bottom: cancelButton.top
                margins: Style.margins.base
            }
            height: 1
            color: Style.colors.lineColor
        }

        TextButton {
            id: cancelButton
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: Style.margins.base
            }
            width: okButton.width
            text: qsTr("Отмена")
            onClicked: root.hide()
        }

        TextButton {
            id: okButton
            anchors {
                right: cancelButton.left
                bottom: parent.bottom
                margins: Style.margins.base
            }
            isDefault: true
            width: Math.max(okButton.implicitWidth, cancelButton.implicitWidth)
            text: qsTr("Ok")
            onClicked: {
                root.hide()
                accepted()
            }
        }
    }

    Keys.onEscapePressed: root.hide()
}

