import QtQuick 2.12

ShaderEffect {
    id: root
    anchors.fill: parent

    property var source
    property var areaTex: Image {
        source: "textures/AreaTex.png"
    }
    property var searchTex: Image {
        source: "textures/SearchTex.png"
    }
    property size rtsize: Qt.size(root.width, root.height)

    vertexShader: "qrc:shaders/SMAA_BlendingWeight.vert"
    fragmentShader: "qrc:shaders/SMAA_BlendingWeight.frag"
}
