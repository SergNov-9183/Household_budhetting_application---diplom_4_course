import QtQuick
import QtQuick.Layouts

import HomeFinance

Item {
    id: root

    property real listWidth: (width - Style.margins.base) / 2

    Layout.fillHeight: true
    Layout.fillWidth: true

    CategoriesList {
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: root.listWidth
        model: categoriesModel.expenseCategories

    }

    CategoriesList {
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        width: root.listWidth
        model: categoriesModel.incomeCategories
    }
}
