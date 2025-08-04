import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

Item {
    id: exportTab
    property int projectId: -1

    // Use a wrapper for vertical centering
    Rectangle {
        anchors {
            top: parent.top
            topMargin: 420   // adjust to match your tab bar height
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }
        color: "#404040"

        // Center the export column both vertically and horizontally
        Item {
            width: 600
            height: 300 // adjust if you want more/less space
            anchors.centerIn: parent

            Column {
                width: parent.width
                anchors.centerIn: parent
                spacing: 36

                // Title
                Text {
                    text: "Экспорт JSON"
                    font.pixelSize: 44
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

                // File path input + Browse
                Row {
                    width: parent.width
                    spacing: 40
                    Rectangle {
                        width: parent.width - 160
                        height: 54
                        radius: 8
                        color: "#999999"
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 18
                            text: pathField !== "" ? pathField : "Путь к файлу: C:/…"
                            color: pathField !== "" ? "white" : "#dddddd"
                            font.pixelSize: 20
                            elide: Text.ElideLeft
                            width: parent.width - 36
                            wrapMode: Text.NoWrap
                        }
                    }
                    Rectangle {
                        width: 120
                        height: 54
                        radius: 8
                        color: "#815F7A"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: saveDialog.open()
                            cursorShape: Qt.PointingHandCursor
                        }
                        Text {
                            anchors.centerIn: parent
                            text: "Поиск…"
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                        }
                    }
                }

                // Export button
                Rectangle {
                    width: parent.width
                    height: 80
                    radius: 8
                    color: "#8B7492"
                    opacity: (pathField !== "" && projectId >= 0) ? 1 : 0.6
                    MouseArea {
                        anchors.fill: parent
                        enabled: pathField !== "" && projectId >= 0
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            console.log("Export clicked:", pathField, projectId)
                            var result = dbManager.exportProject(projectId, pathField)
                            console.log("Export result:", result)
                        }
                    }
                    Text {
                        anchors.centerIn: parent
                        text: "Экспорт"
                        color: "white"
                        font.pixelSize: 30
                        font.bold: true
                    }
                }
            }
        }
    }

    // Simple property for the file path string
    property string pathField: ""

    FileDialog {
        id: saveDialog
        title: "Save project JSON"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["JSON files (*.json)"]
        fileMode: FileDialog.SaveFile
        onAccepted: pathField = file
    }
}
