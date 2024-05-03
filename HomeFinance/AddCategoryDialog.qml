import QtQuick
import QtQuick.Layouts

import HomeFinance

HFDialogWindow {
    id: root

    property alias name: editor.text
    property int level
    property bool income
    property int parentId
    property string parentCategoryName

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
                text: qsTr("Добовляем в категорию: ")
            }
            HFLabel {
                Layout.fillWidth: true
                text: root.parentCategoryName
            }
        }
        HFLabel { text: qsTr("Имя категории: ") }
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

    onAccepted: globalController.appendCategory(name, level, income, parentId)
}
