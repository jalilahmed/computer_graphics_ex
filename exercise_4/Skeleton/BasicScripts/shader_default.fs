<<<<<<< HEAD
precision mediump float;
varying vec2 vTex;
uniform vec3 color;
uniform float radius;
uniform float time;

void main(void)
{
	float pixel_r = length(vTex);
	if(pixel_r < radius)
		gl_FragColor = vec4(color * cos(pixel_r / radius * 80.0 / 180.0 * 3.14159), 1.0);
	else discard;
=======
precision mediump float;
varying vec2 vTex;
uniform vec3 color;
uniform float radius;
uniform float time;

void main(void)
{
	float pixel_r = length(vTex);
	if(pixel_r < radius)
		gl_FragColor = vec4(color * cos(pixel_r / radius * 80.0 / 180.0 * 3.14159), 1.0);
	else discard;
>>>>>>> 7eb43f3e354fc6e42b07fd0fd39f87f0f94fe155
}