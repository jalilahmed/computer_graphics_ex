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
	vec4 lightSource;
} ;

layout(std140, binding = 0) uniform ConstantBuffer{

	PhongShaderConstData phongShaderConstData; 
};

 in vec3 N;
 in vec3 V;
 in vec3 L;

void main() 
{ 
 
 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO:   
	// Lighting:   compute the diffuse and spekular term
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PhongShaderConstData tmp = phongShaderConstData; 
	
	vec3 color_diffuse = vec3(0);
	color_diffuse = tmp.kd * dot(normalize(N),normalize(L));
	
    vec3 color_specular = vec3(0);	
	vec3 R = normalize((2. * dot(L,N) * normalize(N)) - normalize(L));
	color_specular = tmp.ks * pow((dot(normalize(V),R)),tmp.shininess);

	out0 =  vec4(color_diffuse + color_specular, 1) ;

}
