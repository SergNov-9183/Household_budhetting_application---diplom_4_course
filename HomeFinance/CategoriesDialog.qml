import QtQuick

import HomeFinance

HFDialogWindow {
    id: root
    contentWidth: 600
    contentHeight: 500

    QtObject {
        id: internal

        readonly property real listWidth: (contentWidth - 3 * Style.margins.base) / 2
    }

    FrameItem {
        anchors.fill: parent

        HFLabel {
            id: label
            anchors {
                top: parent.top
                left: parent.left
                margins: Style.margins.base
            }
            font: Style.fonts.b16
            text: qsTr("Категории:")
        }

        BorderLine {
            id: line
            anchors {
                left: parent.left
                top: label.bottom
                topMargin: Style.margins.base
                right: parent.right
            }
        }

        CategoriesList {
            anchors {
                left: parent.left
                top: line.bottom
                bottom: parent.bottom
                margins: Style.margins.base
            }
            width: internal.listWidth
            model: categoriesModel.expenseCategories

        }

        CategoriesList {
            anchors {
                top: line.bottom
                right: parent.right
                bottom: parent.bottom
                margins: Style.margins.base
            }
            width: internal.listWidth
            model: categoriesModel.incomeCategories
        }
    }
}
