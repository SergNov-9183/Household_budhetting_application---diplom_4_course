import QtQuick
import QtQuick.Layouts

import HomeFinance

FrameItem {
    id: root

    property int accountId: model.id

    property OperationsProxyModel operationsProxyModel: OperationsProxyModel {
        model: operationsModel
        accountId: root.accountId
    }

    color: "#000000"

    RowLayout {
        id: title
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            margins: 6
        }

        HFLabel {
            text: qsTr("Дата")
        }
        HFLabel {
            text: qsTr("Категория")
        }
        HFLabel {
            Layout.fillWidth: true
            text: qsTr("Описание")
        }
        HFLabel {
            text: qsTr("Сумма")
        }
    }

    ListView {
        id: listView
        anchors {
            left: parent.left
            top: title.bottom
            right: parent.right
            bottom: parent.bottom
            margins: 6
        }
        model: operationsProxyModel
        clip: true
        delegate: Row {
            HFLabel {
                text: model.date
            }
            HFLabel {
                text: model.categoryId
            }
            HFLabel {
                //Layout.fillWidth: true
                text: model.description
            }
            HFLabel {
                text: model.price
            }
        }
    }
}
