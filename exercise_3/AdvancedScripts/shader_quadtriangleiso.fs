precision mediump float;

uniform vec3 color;
uniform vec2 pointA;
uniform vec2 pointB;
uniform vec2 pointC;

void main(void)
{
	// use the fragment coordinate (gl_FragCoord) and clip it against the triangle that is defined by the three points A,B and C
	// if the fragment lies in the triangle set the gl_FragColor to the uniform color, otherwise discard the fragment (using "dicard;")
	// if the fragment is inside the triangle and in the range of a iso line (within 1 pixel radius from a iso line) blend the color linearly with the color of the iso line (black)
	// draw all iso lines with a multiple of 14 pixels distance
	vec2 uv = gl_FragCoord.xy;
	
	//-------------------Point Comparison with the Edge AB---------------------------------------
	//--> Calculating the Normal of the Edge AB, AC, BC
	vec2 normal_AB = vec2(-(pointB.y-pointA.y),(pointB.x-pointA.x));
	vec2 normal_CA = vec2(-(pointA.y-pointC.y),(pointA.x-pointC.x));
	vec2 normal_BC = vec2(-(pointC.y-pointB.y),(pointC.x-pointB.x));
	
	
	vec2 pointNow_AB = vec2((uv.x - pointA.x),(uv.y - pointA.y));
	vec2 pointNow_BC = vec2((uv.x - pointB.x),(uv.y - pointB.y));
	vec2 pointNow_CA = vec2((uv.x - pointC.x),(uv.y - pointC.y));
	
	float distance_AB_point = (abs(((pointB[1]-pointA[1])*uv[0]) - ((pointB[0]-pointA[0])*uv[1]) + (pointB[0]*pointA[1])-(pointB[1]*pointA[0]))) / (sqrt(pow((pointB[1]-pointA[1]),2.0)+pow((pointB[0]-pointA[0]),2.0)));
	
	float distance_BC_point = (abs(((pointC[1]-pointB[1])*uv[0]) - ((pointC[0]-pointB[0])*uv[1]) + (pointC[0]*pointB[1])-(pointC[1]*pointB[0]))) / (sqrt(pow((pointC[1]-pointB[1]),2.0)+pow((pointC[0]-pointB[0]),2.0)));
	
	float distance_CA_point = (abs(((pointA[1]-pointC[1])*uv[0]) - ((pointA[0]-pointC[0])*uv[1]) + (pointA[0]*pointC[1])-(pointA[1]*pointC[0]))) / (sqrt(pow((pointA[1]-pointC[1]),2.0)+pow((pointA[0]-pointC[0]),2.0)));
	
	
	float temp = min(distance_AB_point,distance_BC_point);
	float min_distance = min(temp, distance_CA_point);
	
	if ((dot(normal_AB,pointNow_AB) >= 0.) 
		&& (dot(normal_CA,pointNow_CA) >= 0.) 
			&& (dot(normal_BC,pointNow_BC) >= 0.))
	{
	if(mod(min_distance,14.) <= 1.0)
	{
			gl_FragColor = vec4(0.,0.,0.,1.);
	}else{gl_FragColor = vec4(color, 1.);}
	}
	else{discard;}

}
