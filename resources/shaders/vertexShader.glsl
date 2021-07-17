#version 100
attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoords;
varying vec4 v_color;
varying vec2 v_texCoords;
uniform mat3 u_transform;

void main(void) {
    v_texCoords = a_texCoords;
    v_color = a_color;
    vec3 worldPosition = vec3(a_position, 1);
    vec3 normalizedPosition = worldPosition * u_transform;
    gl_Position = vec4(normalizedPosition.xy, 0, 1);
    gl_PointSize = 1.0;
}
