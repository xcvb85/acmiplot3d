attribute vec4 vertex;
attribute vec3 normal;

uniform mat4 camera;
uniform mat4 world;
uniform mat4 projection;
uniform mat3 normalMatrix;

varying vec3 vert;
varying vec3 vertNormal;

void main()
{
    vert = vertex.xyz;
    vertNormal = normalMatrix * normal;
    gl_Position = projection * camera * world * vertex;
};
