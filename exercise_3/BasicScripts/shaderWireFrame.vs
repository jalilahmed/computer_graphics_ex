attribute vec2 vVertex;
attribute vec3 barycenter;

//////////////////////////////////////////////////////////////
// Exercise 3.2: Barycentric Coordinates
// create an attribute for the barycentric Coordinate 


//////////////////////////////////////////////////////////////
// Exercise 3.2: Barycentric Coordinates
// create an interpolation variable (varying) to "send data" to the fragment shader
varying vec3 new_color;

void main(void)
{
    gl_Position = vec4(vVertex, 0.0, 1.0);
    //////////////////////////////////////////////////////////////
    // Exercise 3.2: Barycentric Coordinates
    // assign the barycentric coordinate to the varying
    new_color = barycenter;
}
