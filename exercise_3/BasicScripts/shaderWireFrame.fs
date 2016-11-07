precision mediump float;


//////////////////////////////////////////////////////////////
// Exercise 3.2: Barycentric Coordinates
// create an interpolation variable (varying) "receive data" from the vertex shader
varying vec3 new_color;

void main(void)
{
	float epsilon = 0.01;

	//////////////////////////////////////////////////////////////
	// Exercise 3.2: Barycentric Coordinates
	// visualize the barycentric coordinate as color

	gl_FragColor = vec4(new_color, 1.0);


	//////////////////////////////////////////////////////////////
	// Exercise 3.2: Barycentric Coordinates
	// discard fragments that are not considered "edges"

}