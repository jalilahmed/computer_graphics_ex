precision mediump float;

attribute vec3 vVertex;
attribute vec3 vNormal;

uniform mat4 modelMatrix; // model matrix
uniform mat4 cameraMatrix; // camera matrix
uniform mat4 projectionMatrix; // projection matrix

uniform mat4 normalMatrix;

// TODO 6.3a)	Define a varying variable to
//				pass the normal to the fragment
//				shader.
varying vec3 normal;

// TODO 6.3a)	Define a varying variable to
//				pass the world position to the
//				fragment shader.
varying vec3 world_position;


void main(void)
{
	mat4 MVP = projectionMatrix * cameraMatrix * modelMatrix;
	gl_Position = MVP * vec4(vVertex, 1);

	// TODO 6.3a)	Assign the normal to the varying variable. 
	//				Before you do so, transform it from model
	//				space to world space. Use the appropriate
	//				matrix. Do not forget to normalize the normal
	//				afterwards.
	vec4 tmp = normalMatrix * vec4(vNormal,0);
	normal = normalize(tmp.xyz);
	
	
	// TODO 6.3a)	Assign the position to the varying variable. 
	//				Before you do so, transform it from model
	//				space to world space. Use the appropriate
	//				matrix. Do not forget to dehomogenize it 
	//				afterwards.
	tmp = modelMatrix * vec4(vVertex,1);
	for(int i = 0; i < 3 ; i++){
		world_position[i] = tmp[i]/tmp[3];
	}



}
