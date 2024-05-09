import QtQuick
import QtQuick.Layouts

import HomeFinance

HFDialogWindow {
    id: root

    function showUsers() {
        stackLayout.currentIndex = 0
        show()
    }

    contentWidth: 1.2 * about.implicitWidth
    contentHeight: about.implicitHeight + borderLine.height + tabs.implicitHeight + content.implicitHeight + 2 * Style.margins.base

    Row {
        id: about
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }
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

    BorderLine {
        id: borderLine
        anchors {
            left: parent.left
            top: about.bottom
            topMargin: Style.margins.base
            right: parent.right
        }
    }

    TabsRow {
        id: tabs
        anchors {
            left: parent.left
            top: borderLine.bottom
            topMargin: Style.margins.base
            right: parent.right
        }
        contents: stackLayout
        model: ListModel {
            ListElement {
                name: qsTr("Пользователи")
                icon: "Users"
            }
            ListElement {
                name: qsTr("Проект")
                icon: "Project"
            }
        }
    }

    FrameItem {
        id: content
        anchors {
            left: parent.left
            top: tabs.bottom
            right: parent.right
        }
        implicitHeight: Math.max(users.implicitHeight, project.implicitHeight) + 2 * Style.margins.base

        StackLayout {
            id: stackLayout
            anchors {
                fill: parent
                margins: Style.margins.base
            }

            ColumnLayout {
                id: users

                property real indentation: userLabel.implicitWidth / 4

                Layout.fillWidth: true
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
                    Item { implicitWidth: users.indentation }
                    HFLabel {
                        Layout.fillWidth: true
                        text: globalController.userName
                        font: Style.fonts.b14
                    }
                }
                RowLayout {
                    Item { implicitWidth: users.indentation}
                    HFLabel {
                        Layout.fillWidth: true
                        text: globalController.userMiddleName
                        font: Style.fonts.b14
                    }
                }
                RowLayout {
                    Item { implicitWidth: users.indentation}
                    HFLabel {
                        Layout.fillWidth: true
                        text: globalController.userSurName
                        font: Style.fonts.b14
                    }
                }
            }

            ColumnLayout {
                id: project

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
        }
    }
}
