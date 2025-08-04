import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

Window {
    id: window
    visible: true

    width: 1920
    height: 1080

    minimumWidth: 800
    minimumHeight: 600
    color: "#404040"

    property int cardCount: 0
    property string newProjectName: ""
    property string newProjectSummary: ""
    property string newProjectImagePath: ""
    property string selectedProjectTitle: ""
    property int selectedProjectId: -1

    Component.onCompleted: {
        var projects = dbManager.getAllProjects();
        projectModel.clear();
        for (var i = 0; i < projects.length; i++) {
            projectModel.append({
                id: projects[i].id,   // <--- added!
                title: projects[i].name,
                summary: projects[i].summary,
                imagePath: projects[i].imagePath
            });
        }
    }

    ListModel {
        id: projectModel
    }

    function addProject() {
        if (newProjectName !== "") {
            projectModel.append({
                id: -1,    // optional, new project not yet in DB
                title: newProjectName,
                summary: newProjectSummary,
                imagePath: newProjectImagePath
            })
            cardCount++
            newProjectName = ""
            newProjectSummary = ""
            newProjectImagePath = ""
            projectPopup.close()
        }
    }

    Column {
        width: parent.width
        anchors {
            top: parent.top
            left: parent.left
            margins: 20
        }
        spacing: 20

        // — Title Bar —
        Row {
            width: parent.width
            height: 50
            Text {
                text: "Narrative App"
                font.pixelSize: 24
                color: "white"
            }
        }

        // — “Projects” Label —
        Text {
            text: "Проекты"
            font.pixelSize: 24
            color: "white"
        }

        // — Cards Flow —
        Flow {
            width: parent.width
            spacing: 20

            Repeater {
                model: projectModel

                ProjectCard {
                    projectId: model.id
                    title: model.title
                    summary: model.summary
                    imagePath: model.imagePath
                    isNew: false
                }
            }

            // — The + card —
            ProjectCard {
                title: "New"
                isNew: true
                summary: ""
                imagePath: ""

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        projectPopup.itemName = ""
                        projectPopup.itemSummary = ""
                        projectPopup.itemImagePath = ""
                        projectPopup.open()
                    }
                }
            }
        }
    }

    Loader {
        id: screenLoader
        anchors.fill: parent
    }

    // — Centered Popup for entering project data —
    AddItemPopup {
        id: projectPopup
        itemType: "Project"

        onAccepted: {
            if (projectPopup.itemName !== "") {
                // 1. Insert into database:
                dbManager.addProject(
                    projectPopup.itemName,
                    projectPopup.itemSummary,
                    projectPopup.itemImagePath
                );

                // 2. Reload from database to refresh model:
                var projects = dbManager.getAllProjects();
                projectModel.clear();
                for (var i = 0; i < projects.length; i++) {
                    projectModel.append({
                        id: projects[i].id,    // <--- added!
                        title: projects[i].name,
                        summary: projects[i].summary,
                        imagePath: projects[i].imagePath
                    });
                }

                cardCount++;
            }
            projectPopup.close();
        }

        onCancelled: {
            projectPopup.close()
        }
    }

    FileDialog {
        id: imageFileDialog
        title: "Select Project Image"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp *.gif)"]
        onAccepted: {
            newProjectImagePath = file
        }
        onRejected: {
            // do nothing
        }
    }
}
