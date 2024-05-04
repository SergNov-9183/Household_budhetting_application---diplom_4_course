import QtQuick
import QtQuick.Layouts

import HomeFinance

HFDialogWindow {
    id: root

    property alias name: editor.text
    property int type
    property int parentId
    property string parentAccountName

    height: 180
    okEnabled: editor.text.length > 0

    ColumnLayout {
        anchors {
            fill: parent
            margins: Style.margins.base
        }

        RowLayout {
            Layout.fillWidth: true

            HFLabel {
                text: qsTr("Тип счета: ")
            }
            HFLabel {
                Layout.fillWidth: true
                text: root.parentAccountName
            }
        }
        HFLabel { text: qsTr("Имя счета: ") }
        HFEditText {
            id: editor
            Layout.fillWidth: true
        }
    }

    onVisibleChanged: {
        if (visible) {
            editor.activate()
        }
    }

    onAccepted: globalController.appendAccount(name, type, parentId)
}
