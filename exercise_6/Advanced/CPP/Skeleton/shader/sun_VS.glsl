 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 

  
struct SunShaderConstData
{
	mat4 MVP;  
	float t;
};

layout(std140, binding = 2) uniform ConstantBuffer{

	SunShaderConstData sunShaderConstData; 
};

 out vec3 N;
 out vec3 V;
 out vec3 L;

 
void main() {

	 V = vPosition;
	  
	  
	gl_Position =  sunShaderConstData.MVP * vec4(vPosition,1);

}