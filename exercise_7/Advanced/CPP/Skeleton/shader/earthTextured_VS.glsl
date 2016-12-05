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

	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 //
	 // TODO: 7.1
	 //			-Sample the height map "textures[4]" (check the application for the sequence of textures)
	 //			at the proper MIP Level	
	 //			-Scale the vertex positions in object space, transform and assign
	 //			-compute vectors for lighting (for initial testing)
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 	 
	 N =  normalize(mat3(transpose(inverse(phongShaderConstData.V*phongShaderConstData.M)))* vNormal); 
	 uv=vTexCoord;
	 vec4 lpV = phongShaderConstData.V * vec4(phongShaderConstData.lightSource.xyz,1);
		
	 //vec2 dx = dFdx(-vTexCoord);
     //vec2  dy_vtc        = dFdy(TexCoord);
     //float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
     //float mml = log2(delta_max_sqr);
     //float mipmapLevel = max( 0, mml );

	 float height = texture(textures[4],-vTexCoord).x; 	 
	 p = vPosition.xyz;
	 float scale = .18f ;
	 vec3 pos = p *  (1.f + scale * height);
	 pM = vec3(phongShaderConstData.M * vec4(pos,1));
	 pV = vec3(phongShaderConstData.V * vec4(pM,1));
	
	 
	 L = normalize(lpV.xyz  - pV ); 
		 
	 gl_Position =   phongShaderConstData.P *  vec4(pV,1);

}
