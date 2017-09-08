import QtQuick 2.5
import QtQuick.Controls 2.0

ListView {
    id: objectsListView

    clip: true
    focus: true
    property real horizontalMargin: 5
    property real verticalMargin: 5
    property real minimumWidth: 200

    delegate: Item {
        id: rootObjectItemDelegate
        width: objectsListView.width
        height: label.implicitHeight + 5
        property variant    objectData: modelData
        property real       contentWidth: content.width

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
