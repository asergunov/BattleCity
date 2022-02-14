import QtQuick 2.6
import QtQuick.Window 2.2

ShaderEffect {
    id: effect
    property variant source;
    property int iGlobalTime: 0
    property size iResolution: Qt.size(2,2)
    
    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: effect.iGlobalTime = effect.iGlobalTime+100;
    }
    
    vertexShader: "
uniform highp mat4 qt_Matrix;
attribute highp vec4 qt_Vertex;
attribute highp vec2 qt_MultiTexCoord0;
varying highp vec2 coord;
void main() {
  coord = qt_MultiTexCoord0;
  gl_Position = qt_Matrix * qt_Vertex;
}"
    
    fragmentShader: "
varying highp vec2 coord;

uniform int iGlobalTime;
uniform ivec2 iResolution;
uniform sampler2D source;

/// from https://www.shadertoy.com/view/XtlSD7
vec2 CRTCurveUV( vec2 uv )
{
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs( uv.yx ) / vec2( 12.0, 8.0 );
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void DrawVignette( inout vec3 color, vec2 uv )
{
    float vignette = uv.x * uv.y * ( 1.0 - uv.x ) * ( 1.0 - uv.y );
    vignette = clamp( pow( 16.0 * vignette, 0.3 ), 0.0, 1.0 );
    color *= vignette;
}

void DrawScanline( inout vec3 color, vec2 uv )
{
    float scanline 	= clamp( 0.95 + 0.05 * cos( 3.14 * ( uv.y + 0.008 * iGlobalTime ) * 240.0 * 1.0 ), 0.0, 1.0 );
    float grille 	= 0.85 + 0.15 * clamp( 1.5 * cos( 3.14 * uv.x * 640.0 * 1.0 ), 0.0, 1.0 );
    color *= scanline * grille * 1.2;
}

void main() {
    vec3 color;
    // CRT effects (curvature, vignette, scanlines and CRT grille)
    vec2 uv    = coord.xy;
    vec2 crtUV = CRTCurveUV( uv );
    if ( crtUV.x < 0.0 || crtUV.x > 1.0 || crtUV.y < 0.0 || crtUV.y > 1.0 )
    {
        color = vec3( 0.0, 0.0, 0.0 );
    } else {
        color = texture2D(source, crtUV);
        DrawScanline( color, uv );
    }
    DrawVignette( color, crtUV );

    gl_FragColor.xyz 	= color;
    gl_FragColor.w		= 1.0;
}"
}
