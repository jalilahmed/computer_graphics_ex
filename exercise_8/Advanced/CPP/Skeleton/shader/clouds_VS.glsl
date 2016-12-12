 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 
layout(location = 2) in  vec2 vTexCoord; 

 

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


out Data
{
  vec3 N;
  vec2 uv;
  vec3 L;
  vec3 pV;
};

 
void main() {

	 
	 
	 N =  normalize(mat3(transpose(inverse(phongShaderConstData.V*phongShaderConstData.M)))* vNormal);
	  
	 uv=vTexCoord;
	    
	vec3 pos = vPosition; 
	 

	pV = vec3(phongShaderConstData.V *  phongShaderConstData.M * vec4(pos ,1 ));
	vec4 lpV = phongShaderConstData.V * vec4(phongShaderConstData.lightSource.xyz,1);
	
	L = normalize(lpV.xyz  - pV );

	gl_Position =   phongShaderConstData.P *  vec4(pV,1);

}
