"use strict";

///////////////////////////
//// global variables  ////
///////////////////////////

// pixel scale
var pixelScaleCanvas1 = 10;

// line
var line = new Line(new Point(1 / pixelScaleCanvas1, 1 / pixelScaleCanvas1),
                    new Point(190 / pixelScaleCanvas1, 190 / pixelScaleCanvas1),
                    new Color(0, 0, 255));

//////////////
//// gui  ////
//////////////

// event listener for gui
function onChangePixelScaleCanvas1(value) {
    // rescale line
    console.log("Changed Pixel Scale Canvas");

    var s = pixelScaleCanvas1 / value;
    line.startPoint.x = line.startPoint.x * s;
    line.startPoint.y = line.startPoint.y * s;
    line.endPoint.x = line.endPoint.x * s;
    line.endPoint.y = line.endPoint.y * s;
    // set new scaling factor
    pixelScaleCanvas1 = value;
    // rerender scene
    RenderCanvas1();
}

function onMouseDownCanvas1(e) {
    var rect = document.getElementById("canvas1").getBoundingClientRect();
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;

    console.log("onMouseDownCanvas1: " + x + " " + y);

    // set new points
    if (e.ctrlKey) {
        line.endPoint.x = x / pixelScaleCanvas1;
        line.endPoint.y = y / pixelScaleCanvas1;
    }
    else {
        line.startPoint.x = x / pixelScaleCanvas1;
        line.startPoint.y = y / pixelScaleCanvas1;
    }

    // rerender image
    RenderCanvas1();
}


//////////////////////////////
//// bresenham algorithm  ////
//////////////////////////////

function bresenham(image, line) {
    // ensure integer coordinates. Math.floor kind of round off the figure... 45.95 = 45, 45.05 =45
    var x0 = Math.floor(line.startPoint.x);
    var y0 = Math.floor(line.startPoint.y);
    var x1 = Math.floor(line.endPoint.x);
    var y1 = Math.floor(line.endPoint.y);
    var step_y = 1;
    
    if (Math.abs((y1-y0)/(x1-x0))>1){
        x0 = [y0, y0 = x0][0];
        x1 = [y1, y1 = x1][0];
    }
    if (x0 > x1){
        x0 = [x1, x1 = x0][0];
        y1 = [y0, y0 = y1][0];
    }
    if (((y1-y0)/(x1-x0))<0){
       step_y = -1;
    }
    
    //////////////////////////
    ///////   TODO   /////////
    //////////////////////////

    // TODO: compute deltas and update directions
    
    var delta = (x1-x0) - 2*((y1-y0));
    var delta_east = -2*((y1-y0));
    var delta_northeast = 2*((x1-x0)-((y1-y0)));

    // TODO: set initial coordinates
    var x = x0;
    var y = y0;

    // TODO: start loop to set nPixels, 
    var nPixels = Math.max((x1-x0),(y1-y0)); // TODO: think about how many pixels need to be set
    for (var i = 0; i < nPixels; ++i) {
        // TODO: set pixel use the helper function setPixelS()
        var pixel =new Point(x,y);
        setPixelS(image,pixel, new Color(0, 0, 0), pixelScaleCanvas1);
        x = x + 1;
        // TODO: update error
        
        // TODO: update coordinates depending on the error
        if(delta < 0){
            y = y + step_y;
            delta = delta + delta_northeast;
        }
        else{
            delta = delta + delta_east;
        }

    }

    //////////////////////////
    ///////   TODO   /////////
    //////////////////////////
}


//////////////////////////
//// render function  ////
//////////////////////////

function RenderCanvas1() {
    // get canvas handle
    var context = document.getElementById("canvas1").getContext("2d");
    var canvas = context.createImageData(200, 200);

    // clear canvas
    clearImage(canvas, new Color(255, 255, 255));

    // draw line
    bresenham(canvas, line);

    // draw start and end point with different colors
    setPixelS(canvas, line.startPoint, new Color(255, 0, 0), pixelScaleCanvas1);
    setPixelS(canvas, line.endPoint, new Color(0, 255, 0), pixelScaleCanvas1);

    // show image
    context.putImageData(canvas, 0, 0);
}


function setupBresenham(canvas) {
    // execute rendering
    RenderCanvas1();
    // add event listener
    document.getElementById("canvas1").addEventListener('mousedown', onMouseDownCanvas1, false);
}
