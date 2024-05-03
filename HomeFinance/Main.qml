import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import HomeFinance

Window {
    id: root

    property CategoriesModel categoriesModel: CategoriesModel {
        editorController: globalController
    }

    width: 840
    height: 480
    visible: true
    title: qsTr("Home Finance")
    color: Style.colors.backgroundWindow

    LinearGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1C2125" }
            GradientStop { position: 1.0; color: "#21201A" }
        }
    }

    UserFrame {
        id: userInfo
        anchors {
            left: parent.left
            top: parent.top
            margins: Style.margins.base
        }
    }

    BorderLine {
        id: leftArea
        anchors {
            left: userInfo.right
            leftMargin: Style.margins.base
            top: parent.top
            bottom: statusBar.top
        }
        isHorizontal: false
    }

    BorderLine {
        id: userArea
        anchors {
            left: parent.left
            top: userInfo.bottom
            topMargin: Style.margins.base
            right: leftArea.left
        }
    }

    SectionList {
        anchors {
            left: parent.left
            top: userArea.bottom
            right: leftArea.left
            bottom: statusBar.top
            margins: Style.margins.base
        }
        onClicked: (index) => mainArea.currentIndex = index
    }

    FrameItem {
        id: mainFrame
        anchors {
            left: leftArea.right
            top: parent.top
            right: parent.right
            bottom: statusBar.top
            margins: Style.margins.base
        }

        StackLayout {
            id: mainArea
            anchors {
                fill: parent
                margins: Style.margins.base
            }

            AccountsSection   {}
            OperationsSection {}
            CategoriesSection {}
            AnalyticsSection  {}
            SettingsSection   {}
        }
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

    DisableFrame { visible: addCategoryWindow.visible }
    AddCategoryDialog { id: addCategoryWindow }
}
