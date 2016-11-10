<<<<<<< HEAD
precision mediump float;

uniform vec3 clearColor;
uniform float viewportDim; // in x and y direction (aspect ratio=1)!
uniform float time;

varying vec2 vTex;
uniform vec3 color0;
uniform vec3 color1;
uniform float radius;

void main(void)
{
	float distanceToOrbit = abs(length(vTex) - radius) / 2.0  * viewportDim;
	// if(distanceToOrbit < 1.0)
	// {
	// 	float alpha = clamp(distanceToOrbit, 0.0, 1.0);
	// 	gl_FragColor = vec4(mix(color0, clearColor, alpha), 1.0);
	// }
	// else
	{
		discard;
	}
=======
precision mediump float;

uniform vec3 clearColor;
uniform float viewportDim; // in x and y direction (aspect ratio=1)!
uniform float time;

varying vec2 vTex;
uniform vec3 color0;
uniform vec3 color1;
uniform float radius;

void main(void)
{
	float distanceToOrbit = abs(length(vTex) - radius) / 2.0  * viewportDim;
	// if(distanceToOrbit < 1.0)
	// {
	// 	float alpha = clamp(distanceToOrbit, 0.0, 1.0);
	// 	gl_FragColor = vec4(mix(color0, clearColor, alpha), 1.0);
	// }
	// else
	{
		discard;
	}
>>>>>>> 7eb43f3e354fc6e42b07fd0fd39f87f0f94fe155
}