 #version 450
  
layout(location = 0) in  vec3 vPosition; 

 
struct PhongShaderConstData
{
	mat4 M;
	mat4 VP;
	vec4 color;
};

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};
 

 
void main() {

		 
	gl_Position =   phongShaderConstData.VP *  phongShaderConstData.M * vec4(vPosition,1);

}