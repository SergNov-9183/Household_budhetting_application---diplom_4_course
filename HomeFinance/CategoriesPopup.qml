import QtQuick
import QtQuick.Controls

Popup {
    id: root

    signal clicked(int id)

    implicitHeight: 400
    implicitWidth: 300
    background: FrameItem {}

    CategoriesList {
        anchors.fill: parent
        color: "transparent"
        model: categoriesModel.allCategories
        onClicked: (id) => {
            root.close()
            root.clicked(id)
        }
    }
}
