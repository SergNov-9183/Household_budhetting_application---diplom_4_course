import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

FrameItem {
    id: root

    property int accountId: model.id

    property OperationsProxyModel operationsProxyModel: OperationsProxyModel {
        model: operationsModel
        accountId: root.accountId
    }

    color: "#000000"

    CategoriesPopup {
        id: categoriesPopup

        property int id
        property string description
        property real price

        function show(position, id, description, price) {
            categoriesPopup.id = id
            categoriesPopup.description = description
            categoriesPopup.price = price
            x = position.x
            y = position.y
            open()
        }

        onClicked: (categoryId) => {
            if (id > 0) {
                globalController.changeOperation(description, categoryId, price, id)
            }
        }
    }

    RowLayout {
        id: title

        property real categoryWidth: (width - (10 * spacing + dateLabel.width + priceLabel.width + balanceLabel.width + buttonDeleteArea.width + 5)) / 3

        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            margins: 6
        }
        spacing: 4

        HFLabel {
            id: dateLabel
            Layout.preferredWidth: implicitWidth * 3
            horizontalAlignment: Text.AlignHCenter
            font: Style.fonts.b16
            text: qsTr("Дата")
        }
        Rectangle {
            width: 1
            height: dateLabel.implicitHeight * 3 / 5
            color: "white"
        }
        HFLabel {
            id: categoryLabel
            Layout.preferredWidth: title.categoryWidth
            horizontalAlignment: Text.AlignHCenter
            font: Style.fonts.b16
            text: qsTr("Категория")
        }
        Rectangle {
            width: 1
            height: dateLabel.implicitHeight * 3 / 5
            color: "white"
        }
        HFLabel {
            id: descriptionLabel
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font: Style.fonts.b16
            text: qsTr("Описание")
        }
        Rectangle {
            width: 1
            height: dateLabel.implicitHeight * 3 / 5
            color: "white"
        }
        HFLabel {
            id: priceLabel
            Layout.preferredWidth: implicitWidth * 1.5
            horizontalAlignment: Text.AlignHCenter
            font: Style.fonts.b16
            text: qsTr("Сумма")
        }
        Rectangle {
            width: 1
            height: dateLabel.implicitHeight * 3 / 5
            color: "white"
        }
        HFLabel {
            id: balanceLabel
            Layout.preferredWidth: implicitWidth * 1.5
            horizontalAlignment: Text.AlignHCenter
            font: Style.fonts.b16
            text: qsTr("Остаток")
        }
        Rectangle {
            width: 1
            height: dateLabel.implicitHeight * 3 / 5
            color: "white"
        }
        Item {
            id: buttonDeleteArea
            Layout.preferredWidth: Style.size.icon
        }
    }
    BorderLine {
        id: titleLine
        anchors {
            left: parent.left
            top: title.bottom
            right: parent.right
            leftMargin: 6
            rightMargin: 6
        }
        color: "white"
    }

    ListView {
        id: listView
        anchors {
            left: parent.left
            top: titleLine.bottom
            right: parent.right
            bottom: parent.bottom
            margins: 6
        }
        model: operationsProxyModel
        clip: true
        delegate: ItemDelegate {
            id: delegate
            width: listView.width
            height: rowLayout.implicitHeight + delegateLine.implicitHeight + 12
            background: Item {}
            RowLayout {
                id: rowLayout
                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                }
                spacing: 4
                HFLabel {
                    Layout.preferredWidth: dateLabel.width
                    text: model.date
                }
                Rectangle {
                    width: 1
                    height: dateLabel.implicitHeight
                    color: delegateLine.color
                }
                LabelButton {
                    Layout.preferredWidth: categoryLabel.width
                    active: delegate.hovered
                    text: categoriesModel.getFullCategoryName(model.categoryId)
                    elide: Text.ElideLeft
                    onClicked: categoriesPopup.show(mapToItem(root, width, 0), model.id, model.description, model.price)
                }
                Rectangle {
                    width: 1
                    height: dateLabel.implicitHeight
                    color: delegateLine.color
                }
                RowLayout {
                    Layout.fillWidth: true
                    EditLabel {
                        id: descriptionEditor
                        Layout.fillWidth: true
                        text: model.description
                        onRename: (description) => globalController.changeOperation(description, model.categoryId, model.price, model.id)
                    }
                    IconButton {
                        visible: delegate.hovered && !descriptionEditor.edited
                        iconName: "Rename"
                        onClicked: descriptionEditor.startEditing()
                    }
                }
                Rectangle {
                    width: 1
                    height: dateLabel.implicitHeight
                    color: delegateLine.color
                }
                Item {
                    Layout.preferredWidth: priceLabel.width
                    implicitHeight: priceEditor.implicitHeight
                    EditLabel {
                        id: priceEditor
                        anchors {
                            left: parent.left
                            right: priceEditorButton.visible ? priceEditorButton.left : parent.right
                        }
                        validator: RegularExpressionValidator {
                            regularExpression: /\d+([\.,]\d{0,2})?$/
                        }
                        // validator: DoubleValidator {
                        //     notation: DoubleValidator.StandardNotation
                        //     decimals: 2
                        // }
                        text: model.price.toFixed(2).replace(".", ",")
                        onRename: (value) => globalController.changeOperation(model.description, model.categoryId, parseFloat(value.replace(",", ".")), model.id)
                    }
                    IconButton {
                        id: priceEditorButton
                        anchors {
                            right: parent.right
                            verticalCenter: priceEditor.verticalCenter
                        }
                        visible: delegate.hovered && !priceEditor.edited
                        iconName: "Rename"
                        onClicked: priceEditor.startEditing()
                    }
                }
                Rectangle {
                    width: 1
                    height: dateLabel.implicitHeight
                    color: delegateLine.color
                }
                HFLabel {
                    Layout.preferredWidth: balanceLabel.width
                    text: model.balance.toFixed(2).replace(".", ",")
                    color: model.balance < 0 ? "red" : Style.colors.font
                }
                Rectangle {
                    width: 1
                    height: dateLabel.implicitHeight
                    color: delegateLine.color
                }
                Item {
                    Layout.preferredWidth: buttonDeleteArea.width
                    height: 16

                    IconButton {
                        anchors.centerIn: parent
                        visible: delegate.hovered
                        iconName: "Remove"
                        onClicked: globalController.removeOperation(model.id)
                    }
                }
            }
            BorderLine {
                id: delegateLine
                anchors {
                    left: parent.left
                    top: rowLayout.bottom
                    right: parent.right
                    leftMargin: 0
                    topMargin: 6
                    rightMargin: 0
                }
            }
        }
    }
}
