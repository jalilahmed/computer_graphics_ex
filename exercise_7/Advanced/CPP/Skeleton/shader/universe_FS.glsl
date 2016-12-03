#version 450
  
layout(location = 0)  out vec4 out0; // color 
 
  #define M_PI       3.14159265358979323846

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

uniform sampler2D[] textures;

 

 in vec2 tc;
 in vec3 direction;

void main() 
{ 
	vec3 d = normalize(direction); 
	float phi =  atan(d.x,d.z) / (2*M_PI);

	float theta = acos(d.y) / (M_PI) ;  

	vec4 t = texture(textures[0],vec2(phi,theta));


	vec3 lp = vec3(inverse(universeShaderConstData.V) *  vec4(universeShaderConstData.lightSource,1));
	float distanceSun = length(vec3(inverse(universeShaderConstData.V) *  vec4(universeShaderConstData.lightSource,1)));
	lp/=distanceSun;


	float glare =  max(0,  pow(dot(lp, d), max(14,sqrt(sqrt(distanceSun))))); 

	out0 = glare+t*t;

}