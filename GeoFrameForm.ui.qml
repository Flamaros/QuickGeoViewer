import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Qt3D.Core 2.0
import QtQuick.Scene3D 2.0

import Geo.Scene 1.0
import Geo.Object 1.0
import Geo.Point 1.0

Item {
    id: item1
    width: 800
    height: 800

    property alias  userInputTextField: userInput
    property alias  objectsListView: objectsListView
    property alias  animationSlider: animationSlider

    GridLayout {
        id: mainGridLayout
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        flow: GridLayout.LeftToRight
        rows: 2
        columns: 2
        anchors.fill: parent

        Item {
            width: objectsListView.width + 5 * 2
            Layout.fillHeight: true

            ObjectList {
                id: objectsListView
                width: minimumWidth
                anchors.top: parent.top
                anchors.topMargin: verticalMargin
                x: horizontalMargin
                height: parent.height - verticalMargin * 2
            }
        }
        ColumnLayout {
            id: column1
            width: 200
            height: 400
            Layout.fillHeight: true
            Layout.fillWidth: true

            GeoScene {
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

            Slider {
                id: animationSlider
                value: 0
                stepSize: 1
                live: true
                Layout.fillWidth: true
            }

            RowLayout {
                id: row1
                height: userInput.height
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
//                    anchors.right: parent.right
//                    anchors.rightMargin: 0
//                    anchors.left: label1.right
//                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                }
            }

        }
    }
}
