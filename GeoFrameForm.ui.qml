import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

import Qt3D.Core 2.0
import QtQuick.Scene3D 2.0

import Geo.Scene 1.0
import Geo.Object 1.0
import Geo.Point 1.0

Item {
    id: item1
    width: 400
    height: 400

    property alias  userInputTextField: userInput
    property alias  objectsListView: objectsListView

    GridLayout {
        id: mainGridLayout
        flow: GridLayout.LeftToRight
        rows: 2
        columns: 2
        anchors.fill: parent

        Item {
            width: objectsListView.width + 5 * 2
            Layout.fillHeight: true

            ListView {
                id: objectsListView
                width: minimumWidth
                anchors.top: parent.top
                anchors.topMargin: verticalMargin
                x: horizontalMargin
                height: parent.height - verticalMargin * 2
                clip: true
                focus: true
                property real horizontalMargin: 5
                property real verticalMargin: 5
                property real minimumWidth: 200

                Keys.onUpPressed: {
                    currentIndex = Math.min(0, currentIndex - 1)
                }

                Keys.onDownPressed: {
                    currentIndex = Math.max(count, currentIndex - 1)
                }

                delegate: Item {
                    id: rootObjectItemDelegate
                    width: objectsListView.width
                    height: label.implicitHeight + 5
                    property variant    objectData: modelData
                    property real       contentWidth: content.width

//                    border.width: 1
//                    border.color: "black"
//                    color: "Red"
                    MouseArea {
                        width: objectsListView.width - objectsListView.horizontalMargin * 2
                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height
                        onClicked: {
                            objectsListView.currentIndex = index
                        }
                    }

                    Item {
                        id: content

                        width: label.implicitWidth + 5
                        height: parent.height
                        Text {
                            id: label
                            x: 5 / 2
                            text: modelData.label
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                highlight: Rectangle {
                    width: objectsListView.width - objectsListView.horizontalMargin * 2
                    height: objectsListView.currentItem.height + objectsListView.spacing
                    color: "lightsteelblue"
                }
                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    active: true
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
