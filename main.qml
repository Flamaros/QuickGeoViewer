import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

ApplicationWindow {
    id: mainWindow
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
