import QtQuick
import QtQuick.Layouts

HFDialogWindow {
    id: root

    property alias name: editor.text
    property int level
    property bool income
    property int parentId
    property string parentCategoryName

    windowHeight: 180

    ColumnLayout {
        anchors {
            fill: parent
            margins: 10
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
        RowLayout {
            Layout.fillWidth: true

            Item { width: 20 }

            HFEditText {
                id: editor
                Layout.fillWidth: true
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            editor.activate()
        }
    }

    onAccepted: globalController.appendCategory(name, level, income, parentId)
}
