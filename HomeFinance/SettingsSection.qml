import QtQuick
import QtQuick.Layouts

import HomeFinance

Item {
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true

    ColumnLayout {
        id: settings

        property real indentation: userLabel.implicitWidth / 4

        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            margins: Style.margins.base
        }
        spacing: 0

        RowLayout {
            HFLabel {
                id: userLabel
                Layout.fillWidth: true
                text: qsTr("Пользователь:")
                font: Style.fonts.n14
            }
            TextButton {
                isDefault: true
                enabled: false
                text: qsTr("Изменить")
            }
        }
        RowLayout {
            Item { implicitWidth: settings.indentation }
            HFLabel {
                Layout.fillWidth: true
                text: globalController.userName
                font: Style.fonts.b14
            }
        }
        RowLayout {
            Item { implicitWidth: settings.indentation}
            HFLabel {
                Layout.fillWidth: true
                text: globalController.userMiddleName
                font: Style.fonts.b14
            }
        }
        RowLayout {
            Item { implicitWidth: settings.indentation}
            HFLabel {
                Layout.fillWidth: true
                text: globalController.userSurName
                font: Style.fonts.b14
            }
        }
        Item { implicitHeight: Style.height.base / 2 }
        RowLayout {
            HFLabel {
                text: qsTr("Путь к проекту:")
                font: Style.fonts.n14
            }
            HFLabel {
                Layout.fillWidth: true
                text: globalController.settings.projectName
                font: Style.fonts.b14
            }
            TextButton {
                isDefault: true
                text: qsTr("Изменить")
                onClicked: fileDialog.changeProject()
            }
        }
    }

    BorderLine {
        id: topArea
        anchors {
            left: parent.left
            top: settings.bottom
            topMargin: Style.margins.base
            right: parent.right
        }
    }

    Item {
        anchors {
            left: parent.left
            top: topArea.bottom
            right: parent.right
            bottom: parent.bottom
            margins: Style.margins.base
        }

        Row {
            anchors.centerIn: parent
            spacing: 50

            Image {
                source: "qrc:/Images/About.svg"
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                HFLabel {
                    text: qsTr("О программе")
                    font: Style.fonts.n14
                }
                Item {
                    width: Style.width.base
                    implicitHeight: Style.height.base / 3
                }
                HFLabel {
                    Layout.fillWidth: true
                    text: qsTr("Домашние финансы")
                    font: Style.fonts.b14
                }
                HFLabel {
                    text: qsTr("Версия: 1.0.0 (64-bit Windows)")
                    font: Style.fonts.n14
                }
                Item {
                    width: Style.width.base
                    implicitHeight: Style.height.base / 3
                }
                HFLabel {
                    text: qsTr("Выпускная квалификационная работа")
                    font: Style.fonts.n14
                }
                Item {
                    width: Style.width.base
                    implicitHeight: Style.height.base / 3
                }
                HFLabel {
                    text: qsTr("© Новосельцев Сергей Иванович (20ПИ-1)")
                    font: Style.fonts.b14
                }
                HFLabel {
                    text: qsTr("© 2024 Sergey & Company. Все права защищены")
                    font: Style.fonts.n14
                }
            }
        }
    }
}
