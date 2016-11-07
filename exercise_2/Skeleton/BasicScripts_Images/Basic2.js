"use strict";

///////////////////////////
//// global variables  ////
///////////////////////////

// seed point
var seedPoint = new Point(50, 50);

//////////////
//// gui  ////
//////////////

// event listener for gui
function onMouseDownCanvas2(e) {
    var rect = document.getElementById("canvas2").getBoundingClientRect();
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;
    console.log("onMouseDownCanvas2: " + x + " " + y);

    // set new seed point
    seedPoint.x = Math.floor(x);
    seedPoint.y = Math.floor(y);

    // rerender image
    RenderCanvas2();
}

///////////////////////////////
//// flood fill algorithm  ////
///////////////////////////////


function floodFill4(image, pixel, fillColor) {
    // flood fill, considering only the 4 direct neighbors
    // note: border color == fillColor
    /////////////////////////
    ///////   TODO  /////////
    /////////////////////////
    
	// TODO: get the color at pixel location, use getPixel()
    
    var color_pixel = new getPixel(image,pixel);
	// TODO:   base case 
	//       - color channels of the current color are equal to the color channels of the fillColor
	//       - pixel position is out of range 
    
    if(color_pixel.r == fillColor.r && color_pixel.g == fillColor.g && color_pixel.b == fillColor.b){    
        return;
    } else if (color_pixel.r != fillColor.r || color_pixel.g != fillColor.g || color_pixel.b != fillColor.b) {
        console.log("Came in Recrusion");
        //TODO: set pixel color
        setPixel(image,pixel,fillColor);
        // TODO: start recursion (4 neighboring pixels)
        floodFill4(image,new Point(pixel.x+1,pixel.y), fillColor);    
        floodFill4(image,new Point(pixel.x-1,pixel.y), fillColor);   
        floodFill4(image,new Point(pixel.x,pixel.y+1), fillColor);   
        floodFill4(image,new Point(pixel.x,pixel.y-1), fillColor);
        
    }
    /////////////////////////
    ///////   TODO  /////////
    /////////////////////////
}

//////////////////////////
//// render function  ////
//////////////////////////

//function RenderCanvas2() {
    // draw something onto the canvas
    //var context = document.getElementById("canvas2").getContext("2d");
    //context.clearRect(0, 0, 200, 200);
    //context.strokeStyle = "#FF0000"; // note that the color of the boundary has to be the same as the fill color
    //context.strokeStyle
    //context.beginPath();
    //context.moveTo(20, 100);
    //context.lineTo(40, 100);
    //context.lineTo(40, 60);
    //context.lineTo(100, 60);
    //context.lineTo(100, 140);
    //context.lineTo(180, 140);
    //context.stroke();
    //context.strokeRect(20, 20, 160, 160);
    //var canvas = context.getImageData(0, 0, 200, 200);

    // flood fill
    //floodFill4(canvas, seedPoint, new Color(255, 0, 0));

    // draw seed point
    //setPixel(canvas, seedPoint, new Color(0,0, 0));

    // show image
//    context.putImageData(canvas, 0, 0);
//}
function RenderCanvas2() {
    // draw something onto the canvas
    var context = document.getElementById("canvas2").getContext("2d");
    context.clearRect(0, 0, 200, 200);
    var canvas = context.getImageData(0, 0, 200, 200);

    for (var i = 1; i < 20; ++i)
    {
     for (var j = 0; j < 200; ++j)
        {
            setPixel(canvas, new Point(i * 10, j), new Color(255, 0, 0));
            setPixel(canvas, new Point(j, i * 10), new Color(255, 0, 0));
        }
    }

    // flood fill
    floodFill4(canvas, seedPoint, new Color(255, 0, 0));
    
    // draw seed point
    setPixel(canvas, seedPoint, new Color(255, 0, 0));

    // show image
    context.putImageData(canvas, 0, 0);
}
function setupFloodFill(canvas) {
    // execute rendering
    RenderCanvas2();
    // add event listener
    document.getElementById("canvas2").addEventListener('mousedown', onMouseDownCanvas2, false);
}
