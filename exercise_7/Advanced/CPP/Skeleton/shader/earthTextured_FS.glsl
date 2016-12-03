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

 


 in vec3 N;
 in vec3 p;
 in vec3 pM;
 in vec3 pV;
 in vec3 L;
 in vec2 uv;
 
uniform sampler2D[5] textures;

void main() 
{ 
 
	 
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.1
	 //			-Sample the height map "textures[4]" (check the application for the sequence of textures)
	 //			at the proper MIP Level	for SMOOTH normals
	 //			-Scale the vertex positions in object space, transform and assign
	 //			-compute vectors for lighting (for best result)
	 //
	 //			(order of Todos does not necessarily correlate with best order of code)
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
		
	  
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.2
	 //			-Sample day and night texture
	 //			-interpolate between both, check assignment sheet for specifics 
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.3
	 //			-Sample specular map
	 //			-Compute Specular lighting, choose pleasing specular color and shininess
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	 ///////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.4 
	 //			-Sample Cloud Texture
	 //			-if cloud reduce color by cloud value (minus cloud threshold)
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	  

	  // you may completely ignore the variables, maybe they are helpful

	float height;
	vec3 pVscaled; 
	vec3 dpx;
	vec3 dpy; 


 
	 
	
	vec3  Ntangent;
	float dotNL ; 
		  
	vec4 spec ; 

	vec4 clouds  ;


	out0 = texture(textures[0],-uv);
}
