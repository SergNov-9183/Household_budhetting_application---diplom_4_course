import QtQuick
import QtQuick.Layouts

import HomeFinance

Item {
    id: root

    property AccountsModel accountsModel: AccountsModel {
        editorController: globalController
    }


    Layout.fillHeight: true
    Layout.fillWidth: true

    AccountsList {
        anchors.fill: parent
        width: root.listWidth
        model: accountsModel.accounts

    }
}
