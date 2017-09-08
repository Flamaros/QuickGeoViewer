import QtQuick 2.9

GeoFrameForm {
    anchors.fill: parent

    objectsListView.model: application != null ? application.objects : null
    objectsListView.width: {
        var maxWidth = 200.0    // Minimum width
        for (var i = 0; i < objectsListView.contentItem.visibleChildren.length; i++)
            if (objectsListView.contentItem.visibleChildren[i] !== objectsListView.highlightItem)
                maxWidth = Math.max(maxWidth, objectsListView.contentItem.visibleChildren[i].contentWidth)
        return maxWidth + objectsListView.horizontalMargin
    }
    objectsListView.onCurrentItemChanged: {
        for (var i = 0; i < objectsListView.contentItem.visibleChildren.length; i++)
            if (objectsListView.contentItem.visibleChildren[i] !== objectsListView.highlightItem)
                objectsListView.contentItem.visibleChildren[i].objectData.color = "white"
        objectsListView.currentItem.objectData.color = "red"
    }

    animationSlider.from: 0
    animationSlider.to: application != null ? application.objects.length - 1 : 0
    animationSlider.onValueChanged: {
        for (var i = 0; i < application.objects.length; i++)
            application.objects[i].visible =
                    (i == Math.floor(animationSlider.value))
    }

//    Connections {
//       target: objectsListView
//       Keys.onUpPressed: {
//               objectsListView.currentIndex = Math.min(0, currentIndex - 1)
//           }
//     }
//    Connections {
//       target: objectsListView
//       Keys.onDownPressed: {
//           objectsListView.currentIndex = Math.max(count, currentIndex - 1)
//           }
//     }
}
