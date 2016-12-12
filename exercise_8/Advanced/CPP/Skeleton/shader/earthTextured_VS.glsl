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

uniform sampler2D[5] textures;


 out vec3 N;
 out vec3 p;
 out vec3 pM;
 out vec3 pV;
 out vec3 L;
 out vec2 uv;

 
void main() {

	 
	 
	 N =  normalize(mat3(transpose(inverse(phongShaderConstData.V*phongShaderConstData.M)))* vNormal);
	// N =   ( mat3(phongShaderConstData.M)*vNormal);
	 uv=vTexCoord;
	   
	// 
 
	p = vPosition.xyz;
	 float scale = .18f ;
	
	 pM = vec3( phongShaderConstData.M * vec4(p * (1+  scale * textureLod(textures[4],-uv, 3 ).x) , 1));
	 pV = vec3(phongShaderConstData.V *  vec4(pM,1));
	vec4 lpV = phongShaderConstData.V * vec4(phongShaderConstData.lightSource.xyz,1);
	L = normalize(lpV.xyz  - pV );// How to do parallel light
		 
	gl_Position =   phongShaderConstData.P *  vec4(pV,1);

}