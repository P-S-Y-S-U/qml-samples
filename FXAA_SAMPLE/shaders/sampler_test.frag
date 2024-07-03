precision highp float;

varying vec2 qt_TexCoord0;
uniform sampler2D u_texture;

void main() {
  vec4 samplerColor = texture2D(u_texture, qt_TexCoord0);
  //gl_FragColor = vec4( samplerColor.r, samplerColor.g, samplerColor.b, samplerColor.a );
  gl_FragColor = vec4( samplerColor[0], samplerColor[1], samplerColor[2], samplerColor[3] );
  //gl_FragColor = samplerColor;
}
