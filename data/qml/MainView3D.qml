import QtQuick 2.9 as QQ2
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import Geo.Scene 1.0

Scene3D {
    id: scene3D

    property alias scene3D: scene3D
    property alias rootEntity: rootEntity
    property alias camera: camera
    property alias geoScene: geoScene

    focus: true
    aspects: ["input", "logic"]
    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

    Entity {
        id: rootEntity

        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 70.0
            nearPlane : 0.2
            farPlane : 100.0
            position: Qt.vector3d(2.0, -10.0, 10.)
            upVector: Qt.vector3d(0.0, 0.0, 1.0)
            viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
        }

        OrbitCameraController {
            camera: camera
            zoomInLimit: 2.0
            linearSpeed: 50.0
            lookSpeed: 180.0
        }

        components: [
            RenderSettings {
                activeFrameGraph: ForwardRenderer {
                    camera: camera
                    clearColor: Qt.rgba(64 / 255, 128 / 255, 128 / 255, 255 / 255)
                }
            },
            InputSettings { }
        ]

        GeoScene {
            id: geoScene
        }
    }
}
