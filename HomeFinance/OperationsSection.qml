import QtQuick
import QtQuick.Layouts

import HomeFinance

Item {
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true

    TabsRow {
        id: accountsTabs
        anchors {
            left: parent.left
            top: parent.top
            leftMargin: Style.radius.window + Style.margins.base
            margins: Style.margins.base
        }
        contents: stackLayout
        model: accountsModel.accountsTabs
    }

    StackLayout {
        id: stackLayout
        anchors {
            left: parent.left
            top: accountsTabs.bottom
            right: parent.right
            bottom: controlPanel.top
            margins: Style.margins.base
            topMargin: 0
        }
        visible: count > 0

        Repeater {
            model: accountsModel.accountsTabs
            delegate: OperationsList {}
        }
    }

    Item {
        id: controlPanel
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 2 * Style.margins.base + controlPanelRow.implicitHeight

        BorderLine {
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
        }

        RowLayout {
            id: controlPanelRow
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: Style.margins.base
            }

            Item { Layout.fillWidth: true }

            ExpandingButton {
                layoutDirection: Qt.RightToLeft
                image: "qrc:/Images/AppendOperation.svg"
                text: qsTr("Добавить чек")
                onClicked: globalController.appendOperation(stackLayout.children[stackLayout.currentIndex].accountId, categoriesModel.defaultCategory())
            }
        }
    }

    HFLabel {
        anchors.centerIn: parent
        visible: !stackLayout.visible
        text: "Добавьте счет"
    }
}
