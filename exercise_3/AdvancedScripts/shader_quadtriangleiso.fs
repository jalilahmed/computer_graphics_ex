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
	float ep = 0.01;
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
	float w = 1.0 - u - v;
	
	//check if the point is in triangle;
	//if ((u > 0.0)  && (u < 1.0)  && (v > 0.0)  && (v < 1.0)  && (w > 0.0)  && (w < 1.0)  && (u + v + w <= 1.)){gl_FragColor = vec4(1.0,0.,0.,1.0);}
	if ((u == 1. + ep || v == 1. + ep || w == 1.+ ep)  && (u + v + w == 1. + ep)){gl_FragColor = vec4(0.,0.,0.,1.);}
	else{discard;}
	//Drawing a line parallel to the side of trianle AB
	//if (uv == pointA){ gl_FragColor = vec4(0,0,0,1.0);}
	//else{discard;}
}
