#version 450
  
layout(location = 0)  out vec4 out0; // color 
 

 struct PhongShaderConstData
{
	mat4 M;
	mat4 V;
	mat4 P;
	vec3 kd;
	float opacity;
	vec3 ks;	
	float shininess;
	vec3 lightSource;
	int textureCount;
} ;

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};

uniform sampler2D[] textures;



 in vec3 N;
 in vec3 V;
 in vec3 L;
 in vec2 uv;
 
 in float shadowDistance;
void main() 
{ 
	float dinv =(1-shadowDistance);

	//////////////////////////////////////////////////////////////////////////////
	//
	//	TODO 8.1:
	//	Attenuate the opacity of the shadow with the distance
	//	-shadowDistance is 0 for vertices close to the asteroid
	//	
	//	-shadowDistance is 1 for the focal point
	//
	//////////////////////////////////////////////////////////////////////////////

	float shadowOpacity = 0.5f;

	out0 = vec4(0,0,0, shadowOpacity);

}
