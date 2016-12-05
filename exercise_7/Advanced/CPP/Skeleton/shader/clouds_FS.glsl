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
	vec3 lightSource;
	int textureCount;
} ;

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};

uniform sampler2D[] textures;



 in vec3 N;
 in vec3 pV;
 in vec3 L;
 in vec2 uv;

void main() 
{ 
	//discard;
	//return;

	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.4
	 //			-Remove discard & return
	 //			-Sample Cloud Texture
	 //			-Discard non clouds
	 //			-Attenuate Light, see assignment sheet for specifics
	 //			-Create red tint by slower fallof of dotNL using sqrt
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 vec4 cloud;
	 cloud = texture(textures[2],-uv);
	 if (cloud.x < .4f)
	 {
	 discard;
	 }
	 
	 float dotNL = dot(N,L); 
	 if(dotNL < .3f && dotNL > -.3f)
	 {
	 cloud.x = sqrt(cloud.x);
	 }
	 out0 = cloud + dotNL + .3;

}
