function arrow(context, fromx, fromy, tox, toy, text) {
    if (fromx == tox && fromy == toy) return;

    // http://stuff.titus-c.ch/arrow.html
    var headlen = 5;   // length of head in pixels
    var angle = Math.atan2(toy - fromy, tox - fromx);
    context.beginPath();
    context.moveTo(fromx, fromy);
    context.lineTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
    context.stroke();
    if(text)
    {
        var d = [tox - fromx, toy - fromy];
        var l = Math.sqrt(d[0]*d[0]+d[1]*d[1]);
        context.fillText(text, tox + 10 / l * d[0], toy + 10 / l * d[1]);
    }
}

function floatToColor(rgb_float) {
    return [Math.max(Math.min(Math.floor(rgb_float[0] * 255.0), 255), 0),
                Math.max(Math.min(Math.floor(rgb_float[1] * 255.0), 255), 0),
                Math.max(Math.min(Math.floor(rgb_float[2] * 255.0), 255), 0)];
}

function setStrokeStyle(context, rgb_float) {
    var c = floatToColor(rgb_float);
    context.strokeStyle = 'rgb(' + c[0] + ',' + c[1] + ',' + c[2] + ')';
}

function setFillStyle(context, rgb_float) {
    var c = floatToColor(rgb_float);
    context.fillStyle = 'rgb(' + c[0] + ',' + c[1] + ',' + c[2] + ')';
}

function MipMap(texture1D, nLevelMax) {
    var texDim = texture1D.length;
    this.nLevel = Math.max(Math.min(Math.log2(texDim)+1, nLevelMax), 1);
    this.texLevels = new Array(this.nLevel);
    // copy first level
    this.texLevels[0] = new Array(texDim);
    for (var i = 0; i < texDim; ++i) this.texLevels[0][i] = [texture1D[i][0], texture1D[i][1], texture1D[i][2]];
    // TODO: compute mip map pyramid
    // use simple boxfilter to compute next mipmap level
    // assume dimension of texture to be a power of 2
    for (var l = 1; l < this.nLevel; ++l) {
        // 1. compute texture dimension of that level
        	texDim = Math.round(Math.sqrt(texDim));
      
        // 2. allocate array with the right dimension
        	this.texLevels[l] = new Array(texDim);
        // 3. compute the color values of the pixel using a boxfilter
        	var a = 0;
        	var b = 1;
        	for(var i = 0; i < texDim; ++i)
        	{
        	this.texLevels[l][i] = vec3.create();
        	vec3.add(this.texLevels[l][i],this.texLevels[l-1][a],this.texLevels[l-1][a + 1]);
        	vec3.scale(this.texLevels[l][i],this.texLevels[l][i],1.0/2.0);
    		a = a + 2;
    		}
    }
}

MipMap.prototype.sampleNearestNeighbor = function (texCoord, level) {
    texCoord = Math.max(Math.min(texCoord, 1.0), 0.0); // this implements texture clamp mode -> texCoord is in the range of [0,1]
    level = Math.max(Math.min(level, this.nLevel - 1), 0);
    if (this.texLevels[level] == undefined) return [0.0, 0.0, 0.0];
    var idx = Math.max(Math.min(Math.round(texCoord * (this.texLevels[level].length) - 0.5), this.texLevels[level].length-1), 0); // nearest neigbor lookup idx
    return [this.texLevels[level][idx][0], this.texLevels[level][idx][1], this.texLevels[level][idx][2]];
}

var Basic0 = function () {
    var canvas;

    var texture = [ [1.0, 0.0, 0.0], [0.5, 0.5, 0.5], [1.0, 0.0, 1.0],
                    [1.0, 1.0, 0.0], [1.0, 1.0, 1.0], [0.0, 1.0, 0.0],
                    [0.0, 0.0, 1.0], [0.0, 0.0, 0.0], [1.0, 1.0, 0.5]];
    var texDimU = 3;
    var texDimV = 3;

    function sampleNearestNeighbor(texCoord) {
        // this implements texture clamp mode -> texCoord is in the range of [0,1]
        texCoord[0] = Math.max(Math.min(texCoord[0], 1.0), 0.0);
        texCoord[1] = Math.max(Math.min(texCoord[1], 1.0), 0.0);
        // compute nearest neighbor
        var u = Math.round(texCoord[0] * (texDimU - 1));
        var v = Math.round(texCoord[1] * (texDimV - 1));
        var idx = texDimU * v + u;
        return [texture[idx][0], texture[idx][1], texture[idx][2]];
    }

    function interpolateColor(color0, color1, alpha) {
        return [(1.0 - alpha) * color0[0] + alpha * color1[0],
                (1.0 - alpha) * color0[1] + alpha * color1[1],
                (1.0 - alpha) * color0[2] + alpha * color1[2]];
    }

    function sampleBilinear(texCoord) {
        // texture clamp
        var u = Math.max(Math.min(texCoord[0], 1.0), 0.0) * (texDimU - 1);    
        var v = Math.max(Math.min(texCoord[1], 1.0), 0.0) * (texDimV - 1);

        // TODO: Implement bilinear interpolation of the texture stored in the global variable "texture"
        // 1. determin the uv coordinates of the 4 surrounding pixels (use Math.floor / Math.ceil)
 		var above = vec2.fromValues(Math.ceil(u),  Math.floor(v));
		var below = vec2.fromValues(Math.floor(u),  Math.floor(v));
		var left  = vec2.fromValues(Math.floor(u) , Math.ceil(v));
		var right = vec2.fromValues(Math.ceil(u) , Math.ceil(v));
		
        // 2. compute the linear indices of the surrounding pixels (e.g. idx = texDimU * v + u;)
		var idx_above = Math.round(texDimU * above[1] + above[0]);
		var idx_below = Math.round(texDimU * below[1] + below[0]);
		var idx_left  = Math.round(texDimU *  left[1] +  left[0]);
		var idx_right = Math.round(texDimU * right[1] + right[0]);
	
		
        // 3. interpolate linearly in u (use interpolateColor())
        var alpha_u = u - Math.floor(u);
		var color_u = interpolateColor(texture[idx_left],texture[idx_right], alpha_u);
        // 4. interpolate linearly in v (use interpolateColor())
		var color_v = interpolateColor(texture[idx_below],texture[idx_above], alpha_u);
        // replace this line
        var alpha_v = v - Math.floor(v)
        return interpolateColor(color_v,color_u,alpha_v);
    }

    function Render() {
        var context = canvas.getContext("2d");
        context.clearRect(0, 0, 400, 200);
        context.font = "bold 12px Georgia";
        context.textAlign = "center";

        var canvasNN = context.createImageData(200, 200);
        for (var y = 0; y < 200; ++y) {
            for (var x = 0; x < 200; ++x) {
                var texCoord = [x / 199.0, y / 199.0];

                var color = sampleNearestNeighbor(texCoord);

                var pixelIdx = 4 * (y * 200 + x);
                canvasNN.data[pixelIdx + 0] = Math.min(Math.max(0, Math.floor(color[0] * 255)), 255);
                canvasNN.data[pixelIdx + 1] = Math.min(Math.max(0, Math.floor(color[1] * 255)), 255);
                canvasNN.data[pixelIdx + 2] = Math.min(Math.max(0, Math.floor(color[2] * 255)), 255);
                canvasNN.data[pixelIdx + 3] = 255;
            }
        }

        var canvasBI = context.createImageData(200, 200);
        for (var y = 0; y < 200; ++y) {
            for (var x = 0; x < 200; ++x) {
                var texCoord = [x / 199.0, y / 199.0];

                var color = sampleBilinear(texCoord);

                var pixelIdx = 4 * (y * 200 + x);
                canvasBI.data[pixelIdx + 0] = Math.min(Math.max(0, Math.floor(color[0] * 255)), 255);
                canvasBI.data[pixelIdx + 1] = Math.min(Math.max(0, Math.floor(color[1] * 255)), 255);
                canvasBI.data[pixelIdx + 2] = Math.min(Math.max(0, Math.floor(color[2] * 255)), 255);
                canvasBI.data[pixelIdx + 3] = 255;
            }
        }

        context.putImageData(canvasNN, 0, 0);
        context.putImageData(canvasBI, 200, 0);

        // draw texel centers
        for (var y = 0; y < texDimV; ++y) {
            for (var x = 0; x < texDimU; ++x) {
                var coord = [200.0 * x / (texDimU-1.0), 200.0 * y / (texDimV-1.0)];
                context.fillStyle = 'rgb(0,0,0)';
                context.beginPath();
                context.arc(coord[0], coord[1], 3, 0, 2 * Math.PI);
                context.fill();
                context.beginPath();
                context.arc(200 + coord[0], coord[1], 3, 0, 2 * Math.PI);
                context.fill();
            }
        }
        context.beginPath();
        context.moveTo(200, 0);
        context.lineTo(200, 200);
        context.stroke();
        context.fillText("Nearest Neighbor", 100, 15);
        context.fillText("Bilinear", 300, 15);
    }

    return {
        start: function (_canvas) {
            canvas = _canvas;
            Render();
        }
    }
}()

var Basic1 = function () {
    var canvas;
    var nPixels = 2;

    function PerspectiveProjection2D(eye, imagePlane, point2D) {
        var p = [point2D[0] - eye[0], point2D[1] - eye[1]];
        var f = imagePlane - eye[1];
        return f * p[0] / p[1];
    }

    function ProjectPointOntoSurfaceLine(eye, line_x, line_minZ, line_maxZ, point2D) {
        var z_intersect = eye[1] + (line_x - eye[0]) / (point2D[0] - eye[0]) * (point2D[1] - eye[1]);
        var texCoord = (z_intersect - line_minZ) / (line_maxZ - line_minZ);
        return [line_x, z_intersect, texCoord];
    }

    function Render() {
        var context = canvas.getContext("2d");
        context.clearRect(0, 0, 600, 300);
        context.font = "bold 12px Georgia";
        context.textAlign = "center";

        // texture
        var texture1D = [[1.0, 0.0, 0.0], [1.0, 1.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]];

        // mip map
        var mipmap = new MipMap(texture1D, 3);

        // line surface
        var line_x = 270;
        var line_minZ = 150;
        var line_maxZ = 575;
        var nSamples = texture1D.length;

        // draw texture onto surface
        for (var i = 0; i < nSamples; ++i) {
            var texCoord0 = i / nSamples;
            var texCoord1 = (i + 1.0) / nSamples;
            var alpha_start = i / nSamples;
            var alpha_end = (i + 1.0) / nSamples;
            var p0 = [line_x, Math.floor((1.0 - texCoord0) * line_minZ + texCoord0 * line_maxZ)];
            var p1 = [line_x, Math.floor((1.0 - texCoord1) * line_minZ + texCoord1 * line_maxZ)];

            // draw the line segment
            context.beginPath();
            setStrokeStyle(context, [0, 0, 0]);
            context.lineWidth = 2;
            context.moveTo(p0[1], p0[0]);
            context.lineTo(p1[1], p1[0]);
            context.stroke();

            for (var level = 0; level < mipmap.nLevel; ++level) {
                var color = mipmap.sampleNearestNeighbor(0.5 * (texCoord0 + texCoord1), level);
                setStrokeStyle(context, color);
                // draw the line segment
                context.beginPath();
                context.lineWidth = 4;
                context.moveTo(p0[1], p0[0] + 3 + level * 5);
                context.lineTo(p1[1], p1[0] + 3 + level * 5);
                context.stroke();
            }

            // draw auxiliary line between this and the next line segment
            context.beginPath();
            setStrokeStyle(context, [0, 0, 0]);
            context.lineWidth = 2;
            context.moveTo(p0[1], p0[0] - 6);
            context.lineTo(p0[1], p0[0] + 6);
            if (i + 1 == texture1D.length) {
                context.moveTo(p1[1], p1[0] - 6);
                context.lineTo(p1[1], p1[0] + 6);
            }
            context.stroke();

            context.fillText(texCoord0, p0[1], p0[0] + 28);
            if (i + 1 == texture1D.length) {
                context.fillText(texCoord1, p1[1], p1[0] + 28);
            }
        }
        context.fillText("texture coord.: ", 50, 270 + 28);
        context.fillText("surface", line_maxZ, line_x - 4);

        // eye
        var eye = [4, 4];

        // draw eye
        context.fillStyle = 'rgb(0,0,0)';
        context.beginPath();
        context.arc(eye[1], eye[0], 4, 0, 2 * Math.PI);
        context.fill();

        // draw axis
        arrow(context, eye[1], eye[0], eye[1] + 150, eye[0]);
        arrow(context, eye[1], eye[0], eye[1], eye[0] + 150);
        context.fillText("X", eye[1] + 11, eye[0] + 150);
        context.fillText("Z", eye[1] + 150, eye[0] + 14);

        // image plane
        var imagePlane = 100;
        context.fillStyle = 'rgb(0,0,0)';
        context.lineWidth = 1;
        context.fillText("image plane", imagePlane, 210);
        context.strokeStyle = 'rgb(100,100,100)';
        context.beginPath();
        context.setLineDash([1, 1]);
        context.moveTo(imagePlane, eye[0]);
        context.lineTo(imagePlane, 200);
        context.stroke();
        context.setLineDash([1, 0]);

        // evaluate pixels
        var pixel_minX = 50;
        var pixel_maxX = 175;
        for (var i = 0; i < nPixels; ++i) {
            // pixel coordinates in world space
            var pixelTop = [pixel_minX + (i / nPixels) * (pixel_maxX - pixel_minX), imagePlane];
            var pixelBottom = [pixel_minX + ((i + 1.0) / nPixels) * (pixel_maxX - pixel_minX), imagePlane];
            var pixelCenter = [0.5 * (pixelTop[0] + pixelBottom[0]), 0.5 * (pixelTop[1] + pixelBottom[1])];

            // pixel projected onto the surface
            // ProjectPointOntoSurfaceLine projects a point onto the given surface
            // ProjectPointOntoSurfaceLine returns [x, z, texCoord]
            var pixelTop_proj = ProjectPointOntoSurfaceLine(eye, line_x, line_minZ, line_maxZ, pixelTop);
            var pixelBottom_proj = ProjectPointOntoSurfaceLine(eye, line_x, line_minZ, line_maxZ, pixelBottom);
            var pixelCenter_proj = ProjectPointOntoSurfaceLine(eye, line_x, line_minZ, line_maxZ, pixelCenter);
			
            // TODO: determin level, based on the footprint of the pixel
            // 1. use pixelBottom_proj[2] and pixelTop_proj[2] to determin the footprint of the pixel on the texture	
	      	var footprint = pixelTop_proj[2] - pixelBottom_proj[2]; 
            // 2. determin the mip map level where the texel size is larger than the pixel footprint
       		for (var j = 0; j < mipmap.nLevel; ++j)
       		{
       		if(1/mipmap.texLevels[j].length > footprint)
       		{
       		level = j;
       		break;
       		}
       		}
            // read color from the mip map pyramid
            var color = mipmap.sampleNearestNeighbor(pixelCenter_proj[2], level);
			
            // draw pixel
            context.beginPath();
            setStrokeStyle(context, color);
            context.lineWidth = 4;
            context.moveTo(pixelTop[1], pixelTop[0]);
            context.lineTo(pixelBottom[1], pixelBottom[0]);
            context.stroke();

            // draw projection lines
            context.strokeStyle = 'rgb(200,200,200)';
            context.beginPath();
            context.lineWidth = 1;
            context.setLineDash([3, 3]);
            context.moveTo(eye[1], eye[0]);
            context.lineTo(pixelTop_proj[1], pixelTop_proj[0]);
            context.moveTo(eye[1], eye[0]);
            context.lineTo(pixelBottom_proj[1], pixelBottom_proj[0]);
            context.stroke();
            context.beginPath();
            context.strokeStyle = 'rgb(0,0,0)';
            context.moveTo(eye[1], eye[0]);
            context.lineTo(pixelCenter_proj[1], pixelCenter_proj[0]);
            context.stroke();
            context.setLineDash([1, 0]);

            // draw auxiliary line between this and the next line segment
            context.beginPath();
            setStrokeStyle(context, [0, 0, 0]);
            context.lineWidth = 2;
            context.moveTo(pixelTop[1] - 6, pixelTop[0]);
            context.lineTo(pixelTop[1] + 6, pixelTop[0]);
            context.moveTo(pixelBottom[1] - 6, pixelBottom[0]);
            context.lineTo(pixelBottom[1] + 6, pixelBottom[0]);
            context.stroke();
        }
        context.lineWidth = 1;
    }

    return {
        start: function (_canvas) {
            canvas = _canvas;
            Render();
        },
        onChangeNPixels: function (value) {
            nPixels = value;
            Render();
        }
    }
}()
