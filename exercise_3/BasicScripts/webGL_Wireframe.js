function webGLStart(canvas) {

    var gl = canvas.getContext("experimental-webgl");
    if (!gl) alert("Could not initialise WebGL, sorry :-(\nTo enable WebGL support in your browser, go to about:config and skip the warning.\nSearch for webgl.disabled and set its value to false.");

    var glVersion = gl.getParameter(gl.VERSION);
    var glslVersion = gl.getParameter(gl.SHADING_LANGUAGE_VERSION);
    console.log("GL Version: \t" + glVersion);
    console.log("GLSL Version: \t" + glslVersion);

    gl.viewport(0, 0, canvas.width, canvas.height);


    //////////////////////////////////////////////////////////////
    // Exercise 3.2: Barycentric Coordinates
    // add the barycentric coordinats b0[1,0,0], b1[0,1,0] and b3[0,0,1]
    // to the positions of each vertex
    // the layout should be as follows
    // [p0x,p0y,b0x,b0y,b0z,p1x...

    var vertices = [    -.5, -.5,1,0,0,
                        .5, -.5,0,1,0,
                        0, .5, 0,0,1];

    var indices = [0, 1, 2];

    var vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    var ibo = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);


    var fragmentShader = getShader(gl, "shaderWireFrame-fs");
    var vertexShader = getShader(gl, "shaderWireFrame-vs");

    var shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert("Could not initialise shaders");
    }

    gl.useProgram(shaderProgram);

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);


    //////////////////////////////////////////////////////////////
    // Exercise 3.2: Barycentric Coordinates
    // create an attribute for the barycentric Coordinate
    // enable the attribute
    // beware to set the correct NUMBER of elements, STRIDE and OFFSET

    //BEWARE: YOU MUST CHANGE THE VERTEX STRIDE FOR THE POSITION AS WELL

    var attrVertexPosition = gl.getAttribLocation(shaderProgram, "vVertex");
    gl.enableVertexAttribArray(attrVertexPosition);
    gl.vertexAttribPointer(attrVertexPosition, 2, gl.FLOAT, false, 20, 0);

    
    
    var attrVertexColor = gl.getAttribLocation(shaderProgram, "barycenter");
    gl.enableVertexAttribArray(attrVertexColor);
    gl.vertexAttribPointer(attrVertexColor, 3, gl.FLOAT, false, 20, 8);
    
    
    gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT, 0);
}
