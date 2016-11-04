precision mediump float;

uniform vec3 color;
uniform vec2 pointA;
uniform vec2 pointB;
uniform vec2 pointC;

void main(void)
{
	// use the fragment coordinate (gl_FragCoord) and clip it against the triangle that is defined by the three points A,B and C
	// if the fragment lies in the triangle set the gl_FragColor to the uniform color, otherwise discard the fragment (using "dicard;")
	
	vec2 uv = gl_FragCoord.xy;
	
	//Compute Vectors
	vec2  v0 = pointC - pointA;
	vec2  v1 = pointB - pointA;
	vec2  v2  = uv - pointA;
	
	// Compute Dot Products
	float dot00 = dot(v0,v0);
	float dot01 = dot(v0,v1);
	float dot02 = dot(v0,v2);
	float dot11 = dot(v1,v1);
	float dot12 = dot(v1,v2);
	
	//Compute Barycentric Coordinates
	float invDenom = 1.0/(dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	
	
	//check if the point is in triangle;
	if ((u >= 0.0) &&
		(v >= 0.0) &&
		(u + v < 1.)){
	gl_FragColor = vec4(0.,0.,0.,1.0);
	}
	else{discard;}
	
}
