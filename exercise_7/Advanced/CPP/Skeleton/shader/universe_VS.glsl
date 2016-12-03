 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 
layout(location = 2) in  vec2 vTexCoord; 

 
 struct UniverseShaderConstData
{ 
	mat4 V;
	mat4 P;
	vec3 lightSource;
	int textureCount;
} ;

layout(std140, binding = 3) uniform ConstantBuffer{

	UniverseShaderConstData universeShaderConstData; 
};

 out vec2 tc;
 out vec3 direction;
 
void main() {

	 tc = .5*vPosition.xy+vec2(.5);
	direction =   transpose( mat3(universeShaderConstData.V) ) * normalize(vec3(vPosition.xy, 1));	
	gl_Position =  universeShaderConstData.P * vec4(vPosition.xy, 1,1);
	 gl_Position /= gl_Position.w;
	 gl_Position.z = 1;
	 gl_Position.w = 1; 
}