import QtQuick 2.5

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
}
