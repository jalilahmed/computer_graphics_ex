#version 450
  
layout(location = 0)  out vec4 out0; // color 
 
 
struct PhongShaderConstData
{
	mat4 M;
	mat4 VP;
	vec4 color;
};

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};
 

void main() 
{ 
  

	out0 =  phongShaderConstData.color;

}