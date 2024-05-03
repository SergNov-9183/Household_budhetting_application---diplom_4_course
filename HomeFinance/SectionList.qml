import QtQuick
import QtQuick.Layouts

ListView {
    id: root

    signal clicked(int index)

    model: internal.sections
    spacing: 4

    QtObject {
        id: internal
        property ListModel sections: ListModel {
            ListElement {
                display: qsTr("Счета")
                icon: "Accounts"
            }
            ListElement {
                display: qsTr("Операции")
                icon: "Operations"
            }
            ListElement {
                display: qsTr("Категории")
                icon: "Categories"
            }
            ListElement {
                display: qsTr("Аналитика")
                icon: "Analytics"
            }
            ListElement {
                display: qsTr("Настройки")
                icon: "Settings"
            }
        }
    }

    delegate: ListViewDelegate {
        Item {
            width: Style.size.icon
            height: Style.size.icon

            Image {
                anchors.centerIn: parent
                source: "qrc:/Images/" + model.icon + ".svg"
            }
        }

        HFLabel {
            Layout.fillWidth: true
            text: model.display
        }
    }
    onCurrentIndexChanged: clicked(currentIndex)

    Component.onCompleted: root.currentIndex = 1
}
