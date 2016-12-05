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
		vec2 textureCoord = uv * textureSize(textures[4], 0);
		vec2  dx_vtc = dFdx(textureCoord);
        vec2  dy_vtc = dFdy(textureCoord);
        float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
        float mml = log2(delta_max_sqr);
        float mipmapLevel = max(0, mml );
		
		
		float height = textureLod(textures[4],-uv,mipmapLevel).x;  
		float scale = .18f;
		vec3 position =  p *  (1.f + scale * height);
		
		vec3 pVscaled = vec3(phongShaderConstData.M * vec4(position,1));
		pVscaled = vec3(phongShaderConstData.V * vec4(pVscaled,1));
		
		vec4 pVl = phongShaderConstData.V * vec4(phongShaderConstData.lightSource.xyz,1);
		
		vec3 new_L = normalize(pVl.xyz - pVscaled); 
		
		vec3 dpx = dFdx(pVscaled); 
		vec3 dpy = dFdy(pVscaled); 
		
		vec3  Ntangent = normalize(cross(dpx,dpy)); 
		
		float dotNL = dot(Ntangent,new_L); 
	  
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.2
	 //			-Sample day and night texture
	 //			-interpolate between both, check assignment sheet for specifics 
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	  vec4 day =   texture(textures[0],-uv);
	  vec4 night = texture(textures[1],-uv);
	  vec4 color = mix(night,day,clamp(((dotNL + 2.f)/4.f),0.f,1.f));
	 
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.3
	 //			-Sample specular map
	 //			-Compute Specular lighting, choose pleasing specular color and shininess
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 vec4 specSample = texture(textures[3],-uv);
	 vec3 cameraPosition = (inverse(phongShaderConstData.V) * vec4(0,0,0,1)).xyz;
	 vec3 v = normalize( pVscaled - cameraPosition); //if cameraPosition - pVScaled then the opposite side of the earth is lumitaed with sunlight. and side faceing sun is black.
	 vec3 r = normalize(reflect(L,Ntangent)); 
	 vec4 color_spec = specSample * pow(max(0.0,dot(v,r)),1000.f); // the shininess make a ring aroung the earth, size decreased as shininess is increased
	 ///////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.4 
	 //			-Sample Cloud Texture
	 //			-if cloud reduce color by cloud value (minus cloud threshold)
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	

	  // you may completely ignore the variables, maybe they are helpful
	 vec4 cloud =  texture(textures[2],-uv);
	 if (cloud.x > .4f)
	 {
	 cloud = cloud - (cloud - .4f);
	 };
		  
	 out0 = color + color_spec +  dotNL - cloud;
}
