attribute vec4 qt_Vertex;
attribute vec2 qt_MultiTexCoord0;

uniform mat4 qt_Matrix;

varying vec2 v_texCoords;

void main() {
 gl_Position = qt_Matrix * qt_Vertex;
 v_texCoords = qt_MultiTexCoord0;
}
