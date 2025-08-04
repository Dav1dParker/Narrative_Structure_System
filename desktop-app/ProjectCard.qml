import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects
//import QtGraphicalEffects 1.15

Rectangle {
    id: card
    width: 280
    height: 400
    radius: 24
    color: isNew ? "#666666" : "#815F7A"

    property string title: ""
    property string summary: ""
    property string imagePath: ""
    property bool isNew: false
    property int projectId: -1

    property bool hovered: false

    Behavior on scale {
        NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }
    scale: hovered ? 1.03 : 1.0

    border.width: hovered ? 6 : 0
    border.color: hovered ? "#C9A3CF" : "transparent"

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
            wrapMode: Text.WordWrap
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            maximumLineCount: 3
            elide: Text.ElideRight
            clip: true
        }
    }

    MouseArea {
        id: clickArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: card.hovered = true
        onExited: card.hovered = false

        onClicked: {
            if (!isNew) {
                window.selectedProjectTitle = title
                window.selectedProjectId = projectId
                screenLoader.setSource("ProjectScreen.qml", { selectedProjectId: projectId })
            } else {
                projectPopup.open()
            }
        }
    }
}
