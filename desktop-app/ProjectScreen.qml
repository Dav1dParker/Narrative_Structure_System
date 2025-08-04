import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1

import "." as Popups

Item {
    id: screen
    width: parent.width; height: parent.height

    property int currentTabIndex: 0
    property int selectedProjectId: 0

    ListModel { id: charactersModel }
    ListModel { id: locationsModel }
    ListModel { id: eventsModel }

    function reloadModels() {
        // Characters
        var characters = dbManager.getCharacters(selectedProjectId);
        charactersModel.clear();
        for (var i = 0; i < characters.length; i++) {
            charactersModel.append({
                id: characters[i].id,
                title: characters[i].name,
                summary: characters[i].summary,
                imagePath: characters[i].imagePath
            });
        }
        // Locations
        var locations = dbManager.getLocations(selectedProjectId);
        locationsModel.clear();
        for (i = 0; i < locations.length; i++) {
            locationsModel.append({
                id: locations[i].id,
                title: locations[i].name,
                summary: locations[i].summary,
                imagePath: locations[i].imagePath
            });
        }
        // Events
        var events = dbManager.getEvents(selectedProjectId);
        eventsModel.clear();
        for (i = 0; i < events.length; i++) {
            eventsModel.append({
                id: events[i].id,
                title: events[i].name,
                summary: events[i].summary,
                imagePath: events[i].imagePath
            });
        }
    }

    Component.onCompleted: reloadModels()

    function currentModel() {
        return currentTabIndex === 0 ? charactersModel
            : currentTabIndex === 1 ? locationsModel
            : eventsModel
    }

    function currentAddFunc() {
        // Returns the function to add an item in DB
        if (currentTabIndex === 0)
            return dbManager.addCharacter
        if (currentTabIndex === 1)
            return dbManager.addLocation
        return dbManager.addEvent
    }

    function currentUpdateFunc() {
        if (currentTabIndex === 0)
            return dbManager.updateCharacter
        if (currentTabIndex === 1)
            return dbManager.updateLocation
        return dbManager.updateEvent
    }

    function currentDeleteFunc() {
        if (currentTabIndex === 0)
            return dbManager.deleteCharacter
        if (currentTabIndex === 1)
            return dbManager.deleteLocation
        return dbManager.deleteEvent
    }

    AddItemPopup {
        id: addPopup
        itemType: currentTabIndex === 0 ? "Персонаж" :
                  currentTabIndex === 1 ? "Локация" :
                  currentTabIndex === 2 ? "Событие" : ""

        onAccepted: {
            if (currentTabIndex < 3 && addPopup.itemName !== "") {
                // EDIT
                if (addPopup.editMode && addPopup.editIndex >= 0) {
                    let item = currentModel().get(addPopup.editIndex)
                    // Call DB update
                    currentUpdateFunc()(
                        item.id,
                        addPopup.itemName,
                        addPopup.itemSummary,
                        addPopup.itemImagePath
                    )
                } else {
                    // ADD
                    currentAddFunc()(
                        selectedProjectId,
                        addPopup.itemName,
                        addPopup.itemSummary,
                        addPopup.itemImagePath
                    )
                }
                reloadModels()
            }
            addPopup.close()
            infoPopup.close()
        }

        onCancelled: {
            addPopup.close()
        }
    }

    Popups.InfoPopup {
        id: infoPopup

        onEditRequested: {
            var model = currentModel().get(infoPopup.index)
            addPopup.itemName = model.title
            addPopup.itemSummary = model.summary
            addPopup.itemImagePath = model.imagePath
            addPopup.editMode = true
            addPopup.editIndex = infoPopup.index
            addPopup.open()
        }

        onDeleteRequested: {
            if (infoPopup.index >= 0) {
                let item = currentModel().get(infoPopup.index)
                currentDeleteFunc()(item.id)
                reloadModels()
                infoPopup.close()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#404040"

        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Item { height: 40 }    // Spacer BEFORE top bar

            // Top bar
            Row {
                width: parent.width; height: 50; spacing: 10
                MouseArea {
                    width: 30; height: 30
                    onClicked: screenLoader.source = ""
                    Text {
                        anchors.centerIn: parent
                        text: "<"
                        font.pixelSize: 24
                        color: "white"
                    }
                }
                Text {
                    text: window.selectedProjectTitle
                    font.pixelSize: 32
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle { width: parent.width; height: 1; color: "white" }

            Item { height: 40 }    // Spacer AFTER top bar

            // Tab bar...

            // Tab bar
            Row {
                id: tabsRow
                width: parent.width
                height: 40
                spacing: 0
                Repeater {
                    model: 4
                    delegate: Row {
                        width: tabsRow.width / 4
                        height: tabsRow.height
                        spacing: 0
                        Rectangle {
                            id: tabBg
                            width: tabsRow.width / 4
                            height: tabsRow.height
                            color: "#404040"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: currentTabIndex = index
                            }
                            Text {
                                anchors.centerIn: parent
                                text: index === 0 ? "Персонажи"
                                     : index === 1 ? "Локации"
                                     : index === 2 ? "События"
                                     : "Экспорт"
                                font.pixelSize: 38
                                color: currentTabIndex === index ? "#C9A3CF" : "white"
                                font.bold: currentTabIndex === index
                            }
                        }
                        Rectangle {
                            visible: index < 3
                            width: 1
                            height: tabsRow.height
                            color: "white"
                        }
                    }
                }
            }
            Rectangle { width: parent.width; height: 1; color: "white" }

            Loader {
                width: parent.width
                anchors.topMargin: 20
                sourceComponent: currentTabIndex === 0 ? charactersTab :
                                 currentTabIndex === 1 ? locationsTab :
                                 currentTabIndex === 2 ? eventsTab : exportTab
            }
        }
    }

    // --- CHARACTERS TAB ---
    Component {
        id: charactersTab
        Flow {
            width: parent ? parent.width : 0
            spacing: 20
            Repeater {
                model: charactersModel
                delegate: Card {
                    title: model.title
                    summary: model.summary
                    imagePath: model.imagePath
                    isNew: false

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            infoPopup.index = index
                            infoPopup.titleText = model.title
                            infoPopup.summaryText = model.summary
                            infoPopup.imagePath = model.imagePath
                            infoPopup.open()
                        }
                    }
                }
            }
            Card {
                isNew: true
                title: "Новое"
                summary: ""
                imagePath: ""
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        addPopup.itemName = ""
                        addPopup.itemSummary = ""
                        addPopup.itemImagePath = ""
                        addPopup.editMode = false
                        addPopup.editIndex = -1
                        addPopup.open()
                    }
                }
            }
        }
    }

    // --- LOCATIONS TAB ---
    Component {
        id: locationsTab
        Flow {
            width: parent ? parent.width : 0
            spacing: 20
            Repeater {
                model: locationsModel
                delegate: Card {
                    title: model.title
                    summary: model.summary
                    imagePath: model.imagePath
                    isNew: false

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            infoPopup.index = index
                            infoPopup.titleText = model.title
                            infoPopup.summaryText = model.summary
                            infoPopup.imagePath = model.imagePath
                            infoPopup.open()
                        }
                    }
                }
            }
            Card {
                isNew: true
                title: "Новое"
                summary: ""
                imagePath: ""
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        addPopup.itemName = ""
                        addPopup.itemSummary = ""
                        addPopup.itemImagePath = ""
                        addPopup.editMode = false
                        addPopup.editIndex = -1
                        addPopup.open()
                    }
                }
            }
        }
    }

    // --- EVENTS TAB ---
    Component {
        id: eventsTab
        Flow {
            width: parent ? parent.width : 0
            spacing: 20
            Repeater {
                model: eventsModel
                delegate: Card {
                    title: model.title
                    summary: model.summary
                    imagePath: model.imagePath
                    isNew: false

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            infoPopup.index = index
                            infoPopup.titleText = model.title
                            infoPopup.summaryText = model.summary
                            infoPopup.imagePath = model.imagePath
                            infoPopup.open()
                        }
                    }
                }
            }
            Card {
                isNew: true
                title: "Новое"
                summary: ""
                imagePath: ""
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        addPopup.itemName = ""
                        addPopup.itemSummary = ""
                        addPopup.itemImagePath = ""
                        addPopup.editMode = false
                        addPopup.editIndex = -1
                        addPopup.open()
                    }
                }
            }
        }
    }

    // --- EXPORT TAB (empty) ---
    Component {
        id: exportTab
        ExportTab {
            projectId: selectedProjectId
          }
    }
}
