import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

import Qt3D.Core 2.0
import QtQuick.Scene3D 2.0

import Scene 1.0

Item {
    id: item1
    width: 400
    height: 400

    property alias  userInputTextField: userInput

    GridLayout {
        id: gridLayout1
        flow: GridLayout.LeftToRight
        rows: 2
        columns: 2
        anchors.fill: parent

        ListView {
            id: listView1
            width: 110
            height: 160
            Layout.fillHeight: true
            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }

                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }
            }
            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row2
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    spacing: 10
                }
            }
        }
        Item {
            id: column1
            width: 200
            height: 400
            Layout.fillHeight: true
            Layout.fillWidth: true

            GeoScene {
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: row1.top
                anchors.bottomMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                Layout.fillHeight: true
                Layout.fillWidth: true

                Scene3D {
                    id: scene3D
                    anchors.fill: parent

                    focus: true
                    aspects: ["input", "logic"]
                    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

                    entity: scene3DRootEntity
                }
            }

            Item {
                id: row1
                height: userInput.height
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom

                Label {
                    id: label1
                    text: qsTr("Input :")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                }

                TextField {
                    id: userInput
                    text: qsTr("")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: label1.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                }
            }
        }
    }
}
