import QtQuick 2.12

Item {
    anchors.fill: parent

    property var imageSrc

    SMAANeighbourhoodBlend {
        id: neighbourBlend
        source: imageSrc
        blendTexture: ShaderEffectSource {
            format: ShaderEffectSource.RGBA
            wrapMode: ShaderEffectSource.ClampToEdge
            sourceItem: blendWeight
            hideSource: true
        }
    }

    SMAABlendWeight {
        id: blendWeight
        source: ShaderEffectSource {
            format: ShaderEffectSource.RGBA
            wrapMode: ShaderEffectSource.ClampToEdge
            sourceItem: edgeTex
            hideSource: true
        }
    }

    SMAAEdgeDetection {
        id: edgeTex
        source: imageSrc
    }
}
