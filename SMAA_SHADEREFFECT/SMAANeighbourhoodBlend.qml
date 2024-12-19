import QtQuick 2.12

ShaderEffect {
    id: root
    anchors.fill: parent

    property var source
    property var blendTexture
    property size rtsize: Qt.size(root.width, root.height)

    vertexShader: "qrc:shaders/SMAA_NeighbourhoodBlending.vert"
    fragmentShader: "qrc:shaders/SMAA_NeighbourhoodBlending.frag"
}
