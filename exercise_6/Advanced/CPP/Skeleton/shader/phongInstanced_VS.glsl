 #version 450
  
layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vNormal; 

  
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

struct Transform
{ 
	vec4 rotation;
	vec3 position;
	float scale;
};
 layout(std140, binding = 1) buffer TransformBuffer{
 
 	Transform transformBuffer[]; 
 }; 

 
 vec4 conjugate(vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}

vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}

vec3 rotate(vec4 qr, vec3 position)
{  
  vec4 qr_conj = conjugate(qr);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);
  
  vec4 q_tmp = quat_mult(qr, q_pos);
  qr = quat_mult(q_tmp, qr_conj);
  
  return vec3(qr.x, qr.y, qr.z);
}

 out vec3 N;
 out vec3 V;
 out vec3 L;

void main() {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO:  
	//	Instanced Positions: transform the position using all available data
	//
	// Lighting: transform the Vertices using the available matrices, vectors, skalars and quaternions
	//
	// We require
	// - Normal
	// - View Vector
	// - Light Vector
	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	vec4 p = vec4(vPosition, 1);

 
	vec4 pM =  phongShaderConstData.M * p;
	vec4 pMV = phongShaderConstData.V * pM;
	 
 	 
	gl_Position =  phongShaderConstData.P * pMV;

}