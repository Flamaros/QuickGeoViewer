import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

FocusScope {
    id: item1
    width: 800
    height: 800

    property alias  userInputTextField: userInput
    property alias  objectsListView: objectsListView
    property alias  animationSlider: animationSlider
    property alias  playButton: playButton
    property alias  cumulativeCheckBox: cumulativeCheckBox

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
            Layout.fillHeight: true
            Layout.fillWidth: true

            View3D {
                id: view3D
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    id: playButton

                    property bool isPlaying: false
                    text: isPlaying == false ? qsTr("play") : qsTr("stop")
                }

                CheckBox {
                    id: cumulativeCheckBox
                    text: qsTr("cumulative")
                    checked: false
                }

                Slider {
                    id: animationSlider
                    value: 0
                    stepSize: 1
                    live: true
                    Layout.fillWidth: true
                }
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
