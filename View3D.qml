import QtQuick 2.9 as QQ2
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import Geo.Scene 1.0

Scene3D {
    id: scene3D

    focus: true
    aspects: ["input", "logic"]
    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

    property bool sceneRootAdded: false

    onChildrenChanged: {
        if (sceneRootAdded === false)
            children.push(scene3DRootEntity)
        console.log("onChildrenChanged")
        sceneRootAdded = true
    }

//    entity: scene3DRootEntity

    Entity {
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

        }

        PhongMaterial {
            id: material
        }

        TorusMesh {
            id: torusMesh
            radius: 5
            minorRadius: 1
            rings: 100
            slices: 20
        }

        Transform {
            id: torusTransform
            scale3D: Qt.vector3d(1.5, 1, 0.5)
            rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
        }

        Entity {
            id: torusEntity
            components: [ torusMesh, material, torusTransform ]
        }
    }
}
