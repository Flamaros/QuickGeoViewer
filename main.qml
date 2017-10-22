import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

ApplicationWindow {
    id: mainWindow

    property alias geoFrame: geoFrame

    visible: true
    width: 1024
    height: 768
    title: qsTr("Quick Geo Viewer")

    visibility: Window.Maximized

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: "Clear"
                onClicked: {
                    application.clear()
                }
            }
        }
    }

    GeoFrame {
        id: geoFrame
        anchors.fill: parent

        userInputTextField.onAccepted: {
            if (application.parseUserInput(userInputTextField.text))
                userInputTextField.clear()
            else
                userInputTextField.color = "red"
            userInputTextField.focus = false
        }

        userInputTextField.onTextChanged: {
            userInputTextField.color = "black"
        }
    }
}
