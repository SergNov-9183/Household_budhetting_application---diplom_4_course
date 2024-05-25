import QtQuick
import QtQuick.Controls

import HomeFinance

Item {
    id: root

    property bool edited: false
    property alias text: label.text
    property alias font: label.font
    property alias color: label.color
    property alias validator: editor.validator

    signal rename(string text)

    function finishEditing() {
        root.edited = false
        if (editor.text && editor.text !== root.text) {
            root.rename(editor.text)
        }
    }

    function startEditing() {
        edited = true
        editor.activate()
    }

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
        id: editor
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
                finishEditing()
            }
        }
        onAccepted: root.finishEditing()
        Keys.onEscapePressed: {
            editor.text = root.text
            root.finishEditing()
        }
    }
}
