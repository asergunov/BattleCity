import QtQuick 2.6
import QtQuick.Window 2.2

ShaderEffect {
    property var source
    property real uTime: 0.5
    property size uResolution: Qt.size(1,1)
    NumberAnimation on uTime {
        loops: NumberAnimation.Infinite
        from: 0; to: 2;
        duration: 1000
    }
    
    width: scene.width*2; height: scene.height*2
    
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
            precision highp float;
            varying vec4 vColor;
            varying highp vec2 coord;

            uniform float uTime;
            uniform vec2 uResolution;
            uniform sampler2D source;
            //TE scanline effect
            //some code by iq, extended to make it look right
            //ported to Rajawali by Davhed
            float rand(vec2 co) {
                return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
            }

            void main() {
                vec2 q = coord.xy / uResolution.xy;
                // subtle zoom in/out
                vec2 uv = 0.5 + (q-0.5)*(0.98 + 0.006*sin(0.9*uTime));
                vec3 oricol = texture2D(source,vec2(q.x,q.y)).xyz;
                vec3 col;
                // start with the source texture and misalign the rays it a bit
                // TODO animate misalignment upon hit or similar event
                col.r = texture2D(source,vec2(uv.x+0.003,-uv.y)).x;
                col.g = texture2D(source,vec2(uv.x+0.000,-uv.y)).y;
                col.b = texture2D(source,vec2(uv.x-0.003,-uv.y)).z;
                // contrast curve
                col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);

                //vignette
                col *= 0.6 + 0.4*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);

                //color tint
                col *= vec3(0.9,1.0,0.7);
                //scanline (last 2 constants are crawl speed and size)
                //TODO make size dependent on viewport
                col *= 0.8+0.2*sin(10.0*uTime+uv.y*900.0);

                //flickering (semi-randomized)
                col *= 1.0-0.07*rand(vec2(uTime, tan(uTime)));

                //smoothen
                float comp = smoothstep( 0.2, 0.7, sin(uTime) );
                col = mix( col, oricol, clamp(-2.0+2.0*q.x+3.0*comp,0.0,1.0) );
                gl_FragColor = vec4(col,1.0);
            }"
}
