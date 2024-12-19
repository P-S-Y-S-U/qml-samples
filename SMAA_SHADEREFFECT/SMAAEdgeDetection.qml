import QtQuick 2.12

ShaderEffect {
    id: root
    anchors.fill: parent

    property var source
    property size rtsize: Qt.size(root.width, root.height)

    vertexShader: "qrc:shaders/SMAA_EdgeDetection.vert"
    fragmentShader: "qrc:shaders/SMAA_EdgeDetection.frag"
}
