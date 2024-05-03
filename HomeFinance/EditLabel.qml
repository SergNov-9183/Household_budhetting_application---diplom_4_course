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

    HFEditText {
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
        onVisibleChanged: {
           if (visible) {
               text = root.text
               activate()
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
