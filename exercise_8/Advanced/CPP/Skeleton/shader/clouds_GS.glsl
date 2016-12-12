#version 450
  

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;




in Data
{
  vec3 N;
  vec2 uv;
  vec3 L;
  vec3 pV;
} vertIn[]; 
  

out	vec3 N;
out	vec2 uv;
out	vec3 L;
out	vec3 pV; 
 

void main() 
{  
	//////////////////////////////////////////////////////////////////////////////
	//
	//	TODO 8.2:
	//	Flipping of triangle winding
	//	-compute the FACE normal (has nothing to do with vertwx normal
	//  -flip the winding if the FACE normal faces away 
	//	-(note, you can create an auxiliary variable in the loop to simplify 
	//	the order inversion)
	//
	//////////////////////////////////////////////////////////////////////////////
  	vec3 A = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 B = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 newNormal = normalize(cross(A,B));
 
    if(newNormal.z > 0)
    {
    for(int i = gl_in.length() - 1; i >= 0  ; i--)
    {
    	N	= vertIn[i].N;
		pV	= vertIn[i].pV;
	 	L	= vertIn[i].L;
		uv	= vertIn[i].uv;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
   	}	
    }
    else{
    for(int i = 0; i < gl_in.length(); i++) 
    {
		N	= vertIn[i].N;
		pV	= vertIn[i].pV;
	 	L	= vertIn[i].L;
		uv	= vertIn[i].uv;
        gl_Position = gl_in[i].gl_Position;   
        EmitVertex();
    }
    }
    EndPrimitive();
	 
}
