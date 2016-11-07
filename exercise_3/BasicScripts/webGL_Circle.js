function webGLStart(canvas) {

    var gl = canvas.getContext("experimental-webgl");
    if (!gl) alert("Could not initialise WebGL, sorry :-(\nTo enable WebGL support in your browser, go to about:config and skip the warning.\nSearch for webgl.disabled and set its value to false.");

    var glVersion = gl.getParameter(gl.VERSION);
    var glslVersion = gl.getParameter(gl.SHADING_LANGUAGE_VERSION);
    console.log("GL Version: \t" + glVersion);
    console.log("GLSL Version: \t" + glslVersion);

    gl.viewport(0, 0, canvas.width, canvas.height);


    //////////////////////////////////////////////////////////////
    // Exercise 3.1: Draw Circle
    // create triangles using vertices and indices
    // never store vertex data twice

    var c = [0, 0];
    var r = 0.8;
    var slices = 360;
    
    var vertices = [];
    vertices.push(c[0]);
    vertices.push(c[0]);
    delta_theta = 1*(Math.PI/180);
   for(var i = 0 ; i < 361 ; i++){
       vertices.push((r*Math.cos(0+(i*delta_theta))));
       vertices.push(r*(Math.sin(0+(i*delta_theta))));
   }
   for(var i = 361; i > 1; i--){
       console.log("came here" + i);
       var indices = [];
       indices.push(0);
       indices.push(i);
       indices.push(i-1);
    
    
   /*
   for(var i = 0 ; i < 6 ; i ++){
    var vertices = [];
    var indices = [];
    delta_theta = 60*(Math.PI/180);
    vertices.push(r*(Math.cos(0+(i*delta_theta))));
    vertices.push(r*(Math.sin(0+(i*delta_theta))));
    vertices.push(r*(Math.cos(delta_theta + (i*delta_theta))));
    vertices.push(r*(Math.sin(delta_theta+(i*delta_theta))));
    vertices.push(0);
    vertices.push(0);
    
    indices.push(0);
    indices.push(1);
    indices.push(2); 
    */
    var vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    var ibo = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);


    var vertexShader = getShader(gl, "shader-vs");
    var fragmentShader = getShader(gl, "shader-fs");

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

    var attrVertexPosition = gl.getAttribLocation(shaderProgram, "vVertex");
    gl.enableVertexAttribArray(attrVertexPosition);
    gl.vertexAttribPointer(attrVertexPosition, 2, gl.FLOAT, false, 8, 0);
    
    gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT, 0);
    }   
}
