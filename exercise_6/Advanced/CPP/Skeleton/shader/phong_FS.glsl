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
	vec4 lightSource;
} ;

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};

 in vec3 N;
 in vec3 V;
 in vec3 L;

void main() 
{ 
 
 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO:   
	// Lighting:   compute the diffuse and spekular term
	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 
	out0 =  vec4(phongShaderConstData.kd, 1) ;

}