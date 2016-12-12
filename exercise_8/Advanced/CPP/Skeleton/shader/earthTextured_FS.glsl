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


// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}


 in vec3 N;
 in vec3 p;
 in vec3 pM;
 in vec3 pV;
 in vec3 L;
 in vec2 uv;
 
uniform sampler2D[5] textures;
void main() 
{ 
 
	 
	 // 
	  
	  float distance = length(pV);
	 	 
	 vec3 V =  (-1.f/distance) * pV  ;

	 float height = 1 +  .18f * texture(textures[4],-uv , 1 ).x;
	 vec3 pTexturedV = vec3(phongShaderConstData.V* phongShaderConstData.M * vec4( p *  height,1)); 

	vec3 dpx = dFdx(pTexturedV);
    vec3 dpy = dFdy(pTexturedV); 


 
	 
	
	vec3  Ntangent = normalize(  cross(  (dpx), (dpy) ) );
	    


	float dotNL = dot(normalize(L), normalize(Ntangent) );
	 
	
	float dotNLSphere = max(dot(normalize(L), normalize(N) ),0);
	 
	 

	 out0 =   mix( texture(textures[0],-uv), texture(textures[1],-uv)  , 1 - clamp( ( (dotNL + .2))/.4 ,0,1) ) ;
	
	
	vec3 H = normalize(L+V); 
	float dotHN =  max(dot(normalize(H), normalize(Ntangent) ),0);
	

	  

	 vec4 clouds  = texture(textures[2],-uv)-vec4(.4) ;
	 if (clouds.x > 0)
		out0 = max(vec4(.4*out0), out0 -  sqrt(clouds)    );

	vec4 spec = texture(textures[3],-uv)* max(pow(dotHN,6),0)  ;
	 
	  out0 += spec; 

}