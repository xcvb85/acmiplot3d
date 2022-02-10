attribute highp vec4 vertex;

uniform mediump mat4 camera;
uniform mediump mat4 world;
uniform mediump mat4 projection;

void main(void)
{
	gl_Position = projection * camera * world * vertex;
}
