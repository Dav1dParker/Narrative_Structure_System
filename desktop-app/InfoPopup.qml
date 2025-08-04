import QtQuick 2.15
import QtQuick.Controls 2.15

Popup {
    id: infoPopup
    modal: true
    focus: true
    width: parent.width * 0.8
    height: parent.height * 0.75
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    background: Rectangle {
        anchors.fill: parent
        color: "#815F7A"
        radius: 12
        border.color: "#C9A3CF"
        border.width: 2
    }

    property int index: -1
    property string titleText: ""
    property string summaryText: ""
    property string imagePath: ""

    signal editRequested()
    signal deleteRequested()

    Row {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 30

        // — Left: Image (big, fills height)
        Rectangle {
            id: imgBg
            width: Math.min(parent.height * 1.2, parent.width * 0.42) // 16:9 примерно и максимум 42% ширины
            height: parent.height - 60
            color: "#DADADA"
            radius: 12
            clip: true

            Image {
                anchors.fill: parent
                source: imagePath
                fillMode: Image.PreserveAspectCrop
            }
        }

        // — Right: Title, grey box, buttons
        Column {
            id: rightColumn
            width: parent.width - imgBg.width - 60
            height: parent.height - 60
            spacing: 20

            // Title (fixed height)
            Rectangle {
                width: parent.width
                height: 60
                color: "#815F7A"
                radius: 6

                Text {
                    anchors.centerIn: parent
                    text: titleText
                    font.pixelSize: 40
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Separator
            Rectangle { width: parent.width; height: 2; color: "white" }

            // "Description" label
            Text {
                text: "Описание:"
                font.pixelSize: 26
                color: "white"
            }

            // — Серый скроллируемый блок —
            Rectangle {
                id: descBox
                width: parent.width
                height: Math.max(160, rightColumn.height * 0.5)
                color: "#888888"
                radius: 8
                border.color: "#C9A3CF"
                border.width: 1

                Flickable {
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: descText.paintedHeight + 32
                    flickableDirection: Flickable.VerticalFlick
                    clip: true

                    Text {
                        id: descText
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: 16
                            top: parent.top
                        }
                        text: summaryText
                        font.pixelSize: 24
                        color: "white"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                    }
                }
            }

            // Spacer to push buttons down
            Item { width: 1; height: 1;}

            Row {
                width: parent.width
                height: 60
                spacing: 20

                StyledButton {
                    text: "Изменить"
                    color: "#4D364A"
                    fontSize: 22
                    onClicked: editRequested()
                }

                StyledButton {
                    text: "Удалить"
                    color: "#4D364A"
                    fontSize: 22
                    onClicked: deleteRequested()
                }
            }
        }
    }
}
