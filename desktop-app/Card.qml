import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Effects

Rectangle {
    id: card
    width: 280
    height: 450
    radius: 24
    color: isNew ? "#666666" : "#815F7A"
    property string title: ""
    property string summary: ""
    property string imagePath: ""
    property bool isNew: false

    property bool hovered: false

    Behavior on scale {
        NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }
    Behavior on opacity {
        NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }

    scale: hovered ? 1.03 : 1.0
    opacity: hovered ? 0.95 : 1.0

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: card.hovered = true
        onExited: card.hovered = false
    }

    // Content of the card inside a wrapper for shadow
    Item {
        id: cardContent
        anchors.fill: parent
        anchors.margins: 0

        Column {
            anchors.fill: parent
            spacing: 10
            padding: 20

            Rectangle {
                id: pictureFrame
                width: parent.width - 40
                height: 200
                radius: 24
                color: (isNew || imagePath === "") ? "#DADADA" : "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true

                // 1) Your "real" image lives here, but we make it invisible
                Image {
                    id: sourceItem
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    source: imagePath
                    visible: false
                }

                MultiEffect {
                    anchors.fill: sourceItem
                    source: sourceItem

                    maskEnabled: true
                    maskSource: maskRoundedRect

                    // hard edges:
                    maskSpreadAtMin: 0.0
                    maskThresholdMin: 0.5
                }


                // 3) This is your rounded-rect mask, matching the outer radius
                Item {
                    id: maskRoundedRect
                    anchors.fill: sourceItem
                    visible: false  // only used as a mask
                    layer.enabled: true
                    layer.smooth: true

                    Rectangle {
                        anchors.fill: parent
                        radius: 24        // same corner radius as your card
                        color: "black"
                    }
                }


                // fallback + sign for "New"
                Text {
                    anchors.centerIn: parent
                    text: isNew ? "+" : ""
                    font.pixelSize: 96
                    color: "white"
                    visible: isNew || imagePath === ""
                }
            }

            Text {
                text: isNew ? "Новый" : title
                font.pixelSize: 32
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                width: parent.width - 40
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: summary
                font.pixelSize: 24
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                width: parent.width - 40
                wrapMode: Text.WordWrap
                maximumLineCount: 3
                elide: Text.ElideRight
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true
            }
        }
    }

    DropShadow {
        anchors.fill: cardContent
        horizontalOffset: 0
        verticalOffset: 4
        radius: hovered ? 20 : 12
        samples: 32
        color: "#80000000" // semi-transparent black
        source: cardContent
    }
}
