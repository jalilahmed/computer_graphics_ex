 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 

  
struct SunShaderConstData
{
	mat4 MV;
	mat4 P;  
	float t;
};

layout(std140, binding = 2) uniform ConstantBuffer{

	SunShaderConstData sunShaderConstData; 
};

 out vec3 N;
 out vec3 V;
 out vec3 L;

 out float lengthV;
 
void main() {

	 V =  vPosition;
	  lengthV = length( sunShaderConstData.MV * vec4(vPosition,1) );
	  
	gl_Position =  sunShaderConstData.P *sunShaderConstData.MV * vec4(vPosition,1);

}