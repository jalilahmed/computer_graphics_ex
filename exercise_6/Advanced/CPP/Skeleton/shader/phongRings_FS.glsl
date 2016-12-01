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
 in float r;

void main() 
{ 
	vec3 n = normalize((N));
	vec3 l = normalize(L);
	float d = length(V);
	vec3 v = V/d;
 
	float dotNL = dot(l, n);

	vec3 H = normalize(v+l);

	float dotHN = pow(dot(H,n),16);
	
	float s = (.5+.5*sin(200.f*r))  ;

	out0 =  vec4(mix(dotNL, dotHN, s)) ;

	if(s<.1)
		discard;

}