// AddItemPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

Popup {
    id: addPopup
    modal: true
    focus: true

    width: parent.width * 0.75
    height: parent.height * 0.75
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    background: Rectangle {
        color: "#555555"
        radius: 12
        border.color: "#815F7A"
        border.width: 2
    }

    // Exposed properties
    property alias itemName: nameField.text
    property alias itemSummary: summaryField.text
    property string itemImagePath: ""
    property string itemType: "Item"


    // true = edit, false = create
    property bool editMode: false
    // index of the item being edited (for update), -1 for create
    property int editIndex: -1


    signal accepted()
    signal cancelled()

    Flickable {
        anchors.fill: parent
        contentWidth: width
        contentHeight: form.height
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Column {
            id: form
            width: addPopup.width - 200
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            anchors.top: parent.top
            anchors.topMargin: 20

            // Title
            Label {
                text: "Новый " + itemType
                font.pixelSize: 34
                color: "white"
            }

            // Name
            Label {
                text: "Название:"
                font.pixelSize: 30
                color: "white"
            }
            TextField {
                id: nameField
                placeholderText: "Введите название"
                font.pixelSize: 30
                width: parent.width
                focus: true
            }

            // Summary
            Label {
                text: "Описание:"
                font.pixelSize: 30
                color: "white"
            }
            Flickable {
                width: parent.width
                height: 300
                contentHeight: summaryField.paintedHeight
                clip: true

                ScrollBar.vertical: ScrollBar {}

                TextArea {
                    id: summaryField
                    placeholderText: "Введите описание"
                    font.pixelSize: 30
                    width: parent.width
                    wrapMode: TextArea.Wrap
                    readOnly: false  // if you want it editable
                }
            }




            // Image picker
            Label {
                text: "Выберите изображение:"
                font.pixelSize: 30
                color: "white"
            }
            Row {
                spacing: 10

                StyledButton {
                    text: "Поиск..."
                    fontSize: 28    // Fixed!
                    onClicked: fileDialog.open()
                    width: form.width * 0.25
                }

                Text {
                    text: itemImagePath
                    font.pixelSize: 36
                    color: "lightgray"
                    elide: Text.ElideMiddle
                    width: form.width * 0.75 - 10
                }
            }

            // Action buttons
            Row {
                spacing: 20

                StyledButton {
                    text: editMode ? "Сохранить" : "Добавить"
                    fontSize: 28
                    onClicked: accepted()
                    width: form.width * 0.5 - 10
                }


                StyledButton {
                    text: "Назад"
                    fontSize: 28    // Fixed!
                    onClicked: cancelled()
                    width: form.width * 0.5 - 10
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Выбрать изображение"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp *.gif)"]
        onAccepted: itemImagePath = file
    }
}
