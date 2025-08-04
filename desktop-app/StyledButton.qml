// StyledButton.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property alias text: label.text
    property int fontSize: 18    // Expose font size!

    signal clicked()

    width: 120
    height: 40
    color: "#815F7A"
    radius: 8

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: root.fontSize    // Now correct
        font.bold: true
        color: "white"
    }
}
