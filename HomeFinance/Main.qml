import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import HomeFinance

Window {
    id: root

    property AccountsModel accountsModel: AccountsModel {
        editorController: globalController
    }
    property CategoriesModel categoriesModel: CategoriesModel {
        editorController: globalController
    }
    property OperationsModel operationsModel: OperationsModel {
        editorController: globalController
    }

    width: 1280
    height: 720
    minimumWidth: Math.max(accountsDialog.width, categoriesDialog.width, settingsDialog.width) +
                  expandingMenu.implicitWidth + 4 * Style.margins.base
    minimumHeight: Math.max(accountsDialog.height, categoriesDialog.height, settingsDialog.height) +
                   settingsButton.implicitHeight + statusBar.height + 6 * Style.margins.base
    visible: true
    title: qsTr("Домашние финансы")
    color: Style.colors.backgroundWindow

    Component.onCompleted: globalController.load()

    Connections {
        target: globalController
        function onHasProjectChanged(value) {
            if (value) {
                stackLayout.currentIndex = 0
            }
        }
        function onNeedCreateOrOpenProject() {
            fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog

        property bool change: false

        acceptLabel: qsTr("Открыть/Создать")
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        fileMode: FileDialog.SaveFile
        nameFilters: ["Фыйлы приложения (*.hfproject)", "Фыйлы SQLite (*.sqlite3)", "Все файлы (*)"]
        rejectLabel: change ? qsTr("отмена") : qsTr("Закрыть программу")
        title: qsTr("Необходимо выбрать проект или создать новый")

        function changeProject() {
            change = true
            open()
        }

        onAccepted: globalController.createOrOpen(selectedFile)
        onRejected: {
            if (!change) {
                Qt.quit()
            }
        }
    }

    LinearGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1C2125" }
            GradientStop { position: 1.0; color: "#21201A" }
        }
    }

    ExpandingMenu {
        id: expandingMenu
        anchors {
            left : parent.left
            top: parent.top
            bottom: statusBar.top
        }
        accountsSelected: accountsDialog.visible
        categoriesSelected: categoriesDialog.visible
        expanded: hovered || settingsButton.hovered || disableFrame.visible
        onAccountsButtonClicked: {
            categoriesDialog.hide()
            settingsDialog.hide()
            accountsDialog.show()
        }
        onCategoriesButtonClicked: {
            accountsDialog.hide()
            settingsDialog.hide()
            categoriesDialog.show()
        }
        onUserButtonClicked: {
            accountsDialog.hide()
            categoriesDialog.hide()
            settingsDialog.showUsers()
        }
    }

    ExpandingButton {
        id: settingsButton
        anchors {
            top: parent.top
            right: parent.right
            margins: Style.margins.base
        }
        layoutDirection: Qt.RightToLeft
        expanded: hovered || expandingMenu.hovered || disableFrame.visible
        image: "qrc:/Images/Settings.svg"
        text: qsTr("Настройки")
        selected: settingsDialog.visible
        onClicked: {
            accountsDialog.hide()
            categoriesDialog.hide()
            settingsDialog.show()
        }
    }

    TabsRow {
        anchors {
            left: mainFrame.left
            bottom: mainFrame.top
        }
        contents: stackLayout
        model: ListModel {
            ListElement {
                name: qsTr("Операции")
                icon: "Operations"
            }
            ListElement {
                name: qsTr("Аналитика")
                icon: "Analytics"
            }
        }
    }

    FrameItem {
        id: mainFrame
        anchors {
            left: expandingMenu.right
            top: settingsButton.bottom
            right: parent.right
            bottom: statusBar.top
            margins: Style.margins.base
        }

        StackLayout {
            id: stackLayout
            anchors.fill: parent

            OperationsSection {}
            AnalyticsSection  {}
        }

        DisableFrame {
            id: disableFrame
            visible: !globalController.hasProject || settingsDialog.visible || categoriesDialog.visible || accountsDialog.visible
        }

        AccountsDialog { id: accountsDialog }
        CategoriesDialog { id: categoriesDialog }
        SettingsDialog { id: settingsDialog }
    }

    Item {
        id: statusBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 30

        BorderLine {
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
        }
    }
}
