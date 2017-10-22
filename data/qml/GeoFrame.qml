import QtQuick 2.9

GeoFrameForm {
    id: geoFrameForm

    property alias geoFrameForm: geoFrameForm

    anchors.fill: parent

    Timer {
        id: animationTimer
        repeat: false
        onTriggered: {
            playButton.isPlaying = false
            for (var i = 0; i < application.objects.length; i++)
                application.objects[i].visible = true
        }
    }

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
    animationSlider.onFocusChanged: {
        if (animationSlider.focus == false)
            for (var i = 0; i < application.objects.length; i++)
                application.objects[i].visible = true
    }

    animationSlider.transitions: Transition {
        NumberAnimation {
            properties: "value"
            easing.type: Easing.Linear
            from: 0
            to: animationSlider.to
            duration: playButton.isPlaying ? application.objects.length * 200 : 0
        }
    }

    playButton.onClicked: {
        playButton.isPlaying = !playButton.isPlaying
        animationTimer.interval = application.objects.length * 200 + 5 * 1000
        if (playButton.isPlaying)
        {
            animationTimer.start()
            animationSlider.value = 0
            animationSlider.value = animationSlider.to
        }
        else
            animationTimer.stop()
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
