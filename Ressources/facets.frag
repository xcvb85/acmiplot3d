varying highp vec3 vert;
varying highp vec3 vertNormal;

uniform vec4 color;

void main()
{
	highp vec3 lightPos = vec3(0.0, 0.0, 10000.0);
	highp vec3 L = normalize(lightPos - vert);
	highp float NL = max(dot(normalize(vertNormal), L), 0.0);
	highp vec3 col = clamp(color.rgb * 0.5 + color.rgb * 0.5 * NL, 0.0, 1.0);
	gl_FragColor = vec4(col, 1.0);
};
