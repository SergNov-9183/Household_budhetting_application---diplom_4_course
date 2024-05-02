import QtQuick
import QtQuick.Controls

import HomeFinance

Window {
    id: root

    property CategoriesModel categoriesModel: CategoriesModel {
        editorController: globalController
    }

    width: 640
    height: 480
    visible: true
    title: qsTr("Home Finance")
    color: Style.colors.backgroundWindow

    Item {
        id: tabBar
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: 30
    }

    CategoriesList {
        anchors {
            left: parent.left
            top: tabBar.bottom
            bottom: statusBar.top
            margins: 25
        }
        width: (root.width - 60) / 2
        model: categoriesModel.expenseCategories

    }

    CategoriesList {
        anchors {
            top: tabBar.bottom
            right: parent.right
            bottom: statusBar.top
            margins: 25
        }
        width: (root.width - 60) / 2
        model: categoriesModel.incomeCategories
    }

    Item {
        id: statusBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 30

        Rectangle {
            anchors {
                left: parent.left
                leftMargin: 3
                top: parent.top
                right: parent.right
                rightMargin: 3
            }
            height: 1
            color: Style.colors.lineColor
        }
    }

    AddCategoryDialog {
        id: addCategoryWindow
    }
}
