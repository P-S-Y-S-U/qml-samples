import QtQuick 2.15
import QtQuick3D 1.15
import QtQuick3D.Effects 1.15

import CustomGeometry 1.0

View3D
{
        id: view
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "skyblue"
            backgroundMode: SceneEnvironment.Color
            effects: [
                SMAA {
                    id: smaaEffect
                }
            ]
        }

        /*
        PerspectiveCamera {
            position: Qt.vector3d(0, 200, 300)
            //eulerRotation.x: -30
        }*/

        CustomCamera {
            //eulerRotation.y: -270
            projection: Qt.matrix4x4(
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0
            );
        }

        /*
        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
        }
        */

        Model {
            source: "#Rectangle"
            //eulerRotation.y : 65
            //eulerRotation.x : 25
            //eulerRotation.z : 15
            property variant scaleFactor: 0.02
            scale: Qt.vector3d( scaleFactor, scaleFactor, scaleFactor )
            materials: [ DefaultMaterial {                    
                    diffuseMap: Texture {
                        source: "textures/Text_Sample.png"
                    }
                    lighting: DefaultMaterial.NoLighting
                }
            ]
        }

        /*
        Model {
            source: "#Rectangle"
            materials: [                
            ]
        }*/

}
