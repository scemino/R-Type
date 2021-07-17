// Scanlines shader
#version 100
precision mediump float;
varying vec4 v_color;
varying vec2 v_texCoords;
varying vec2 v_resolution;
uniform sampler2D u_texture;

const vec2 RESOLUTION = vec2(384, 256);
const float LINE_COUNT = 40.0;
const float FREQ = LINE_COUNT * 2.0 * 3.14159;
const float INTENSITY = 0.4;

void main()
{
    vec4 color = texture2D(u_texture, v_texCoords);
    float screenV = gl_FragCoord.y / RESOLUTION.y;
    float scanLine = 1.0 - INTENSITY * (sin(FREQ * screenV) * 0.5 + 0.5);

    gl_FragColor = v_color * vec4(color.rgb * scanLine, color.a);
}
