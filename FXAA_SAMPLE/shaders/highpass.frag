uniform highp sampler2D source; // this item
uniform lowp float qt_Opacity;  // inherited opacity of this item
varying highp vec2 qt_TexCoord0;

void main (void)
{
    mediump vec2 ll, rr, tt, bb, cc;
    highp float offsetX = (1.0/1920.0);
    highp float offsetY = (1.0/1080.0);
    ll = vec2(qt_TexCoord0.x - offsetX, qt_TexCoord0.y);
    rr = vec2(qt_TexCoord0.x + offsetX, qt_TexCoord0.y);
    tt = vec2(qt_TexCoord0.x, qt_TexCoord0.y - offsetY);
    bb = vec2(qt_TexCoord0.x, qt_TexCoord0.y + offsetY);
    cc = vec2(qt_TexCoord0.x, qt_TexCoord0.y);
    //Filter is below
    // 0.0 -1.0  0.0
    //-1.0  9.0 -1.0
    // 0.0 -1.0  0.0
    highp vec4 color = texture2D(source, cc) * 9.0 - texture2D(source, rr) - texture2D(source, bb) - texture2D(source, tt) - texture2D(source, ll);
    gl_FragColor = clamp(color / 5.0, 0.0, 1.0) * qt_Opacity; //5.0 is sum of filter. 9-1-1-1-1 = 5.0
}"