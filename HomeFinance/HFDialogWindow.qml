import QtQuick

import HomeFinance

MouseArea {
    id: root

    default property alias contents: placeholder.data
    property alias windowHeight: window.height
    property alias windowWidth: window.width
    property alias okText: okButton.text
    property alias cancelText: cancelButton.text

    signal accepted()

    function show() { visible = true }
    function hide() { visible = false }

    anchors.fill: parent
    visible: false
    hoverEnabled: true

    Rectangle{
        anchors.fill: parent
        color: "#000000"
        opacity: 0.6
    }


    Rectangle {
        id: window
        anchors.centerIn: parent
        border {
            color: "#0063B1"
            width: 1
        }
        height: 200
        width: 400
        color: "#262626"
        radius: 8

        FrameItem {
            id: placeholder
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
                bottom: line.top
                margins: 10
            }
        }

        Rectangle {
            id: line
            anchors {
                left: parent.left
                right: parent.right
                bottom: cancelButton.top
                margins: 10
            }
            height: 1
            color: Style.colors.lineColor
        }

        TextButton {
            id: cancelButton
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 10
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
                margins: 10
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

    onClicked:         (mouse) => mouse.accepted = true
    onDoubleClicked:   (mouse) => mouse.accepted = true
    onPositionChanged: (mouse) => mouse.accepted = true
    onPressAndHold:    (mouse) => mouse.accepted = true
    onPressed:         (mouse) => mouse.accepted = true
    onReleased:        (mouse) => mouse.accepted = true
    onWheel:           (mouse) => mouse.accepted = true
}

