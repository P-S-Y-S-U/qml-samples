import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Effects 1.15

View3D
{
        id: view
        anchors.fill: parent

        //! [environment]
        environment: SceneEnvironment {
            clearColor: "skyblue"
            backgroundMode: SceneEnvironment.Color
            effects: [
                SMAA {
                    id: smaaEffect
                }
            ]
        }
        //! [environment]

        //! [camera]
        PerspectiveCamera {
            position: Qt.vector3d(0, 200, 300)
            eulerRotation.x: -30
        }
        //! [camera]

        //! [light]
        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
        }
        //! [light]

        //! [objects]
        Model {
            source: "#Sphere"
            materials: [ DefaultMaterial {
                    diffuseColor: "blue"
                }
            ]
        }
        //! [objects]
}
