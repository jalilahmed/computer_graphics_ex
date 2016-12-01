 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 

 
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

 out vec3 N;
 out vec3 V;
 out vec3 L;

 out float r;
 
void main() {

	 
	 r =  length(vPosition);
	 
	 N =   transpose( inverse(mat3(phongShaderConstData.V * phongShaderConstData.M))) * vNormal;
	   
	  vec4 p = phongShaderConstData.V * phongShaderConstData.M * vec4(vPosition,1);
	 V =  -  p.xyz ;

	 vec4 lp = phongShaderConstData.V * phongShaderConstData.lightSource;
	 
	 L = vec3(lp-p);

		 
	gl_Position =   phongShaderConstData.P * phongShaderConstData.V * phongShaderConstData.M * vec4(vPosition,1);

}