import QtQuick
import QtQuick.Controls

import HomeFinance

Item {
    id: root

    property bool edited: false
    property alias text: label.text
    property alias font: label.font
    property alias color: label.color

    signal rename(string text)

    implicitHeight: label.implicitHeight
    implicitWidth: label.implicitWidth

    HFLabel {
        id: label
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        visible: !root.edited
    }

    TextField {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        leftPadding: 0
        rightPadding: 0
        font: root.font
        color: root.color
        visible: root.edited
        background: Item {
            Rectangle {
                id: blue
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: 4
                radius: height/2
                color: "#40BDFF"
            }
            Rectangle {
                anchors {
                    fill: parent
                    bottomMargin: blue.radius
                }
                border {
                    width: 1
                    color: "#2A3332"
                }
                color: "#1D1F1F"
            }
        }
        onVisibleChanged: {
           if (visible) {
               text = root.text
               selectAll()
               forceActiveFocus()
           }
        }
        onFocusChanged: {
            if (!focus) {
                root.edited = false
            }
        }
        onAccepted: {
            root.edited = false
            root.rename(text)
        }

        Keys.onEscapePressed: root.edited = false
    }
}
