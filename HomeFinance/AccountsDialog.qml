import QtQuick

import HomeFinance

HFDialogWindow {
    id: root
    contentWidth: 600
    contentHeight: 500

    FrameItem {
        anchors.fill: parent

        HFLabel {
            id: label
            anchors {
                top: parent.top
                left: parent.left
                margins: Style.margins.base
            }
            font: Style.fonts.b16
            text: qsTr("Счета:")
        }

        BorderLine {
            id: line
            anchors {
                left: parent.left
                top: label.bottom
                topMargin: Style.margins.base
                right: parent.right
            }
        }

        AccountsList {
            anchors {
                left: parent.left
                top: line.bottom
                right: parent.right
                bottom: parent.bottom
                margins: Style.margins.base
            }
            model: accountsModel.accounts
        }
    }
}
