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
in vec2 uv;
in vec3 L;
in vec3 pV; 
 
void main() 
{   
	float dotNL = (dot( normalize(N), normalize(L) ));
	 
	float cloudTex =  texture(textures[0],-uv).x;
	 

		//////////////////////////////////////////////////////////////////////////////
		//
		//	TODO 8.1:
		//	assign the correct opacity value  
		//
		//////////////////////////////////////////////////////////////////////////////

		vec3 cloudColor = cloudTex * vec3(max(sqrt(dotNL +.3), .0), max(dotNL+.3, .0), max(dotNL+.3, .0));
		float cloudOpacity = 0.4f;
		 
 
	out0  =  vec4(cloudColor, cloudOpacity); 
	 
}
