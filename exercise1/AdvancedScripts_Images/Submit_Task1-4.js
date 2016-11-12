
/////////////////////////////////////////////
/////////  Complex Number Helpers  //////////
/////////////////////////////////////////////
function ComplexNumber(re, im) {
    this.re = re;
    this.im = im;
}

function ComplexNumberFromCoords(x, y, canvasID) {
    var canvas = document.getElementById(canvasID);
    this.re = (x / (1.0 * canvas.width) - 0.5);
    this.im = (y / (1.0 * canvas.height) - 0.5);
    if (canvasID == 'julia_canvas') {
        this.re *= 3;
        this.im *= 3;
    } else {
        this.re = this.re * 3 * Math.pow(2, zoom) + center.re;
        this.im = this.im * 2 * Math.pow(2, zoom) + center.im;
    }
}

function mult(x, y) {
    var re = (x.re * y.re - x.im * y.im);
    var im = (x.re * y.im + x.im * y.re);
    return new ComplexNumber(re, im);
}

function add(x, y) {
    var re = (x.re + y.re);
    var im = (x.im + y.im);
    return new ComplexNumber(re, im);
}

function sub(x, y) {
    var re = (x.re - y.re);
    var im = (x.im - y.im);
    return new ComplexNumber(re, im);
}

function abs(x) {
    return Math.sqrt(x.re * x.re + x.im * x.im);
}


/////////////////////////////////
/////////  Magic Math  //////////
/////////////////////////////////
function f_c(z, c) {
    // TODO 1.4a):      Compute the result of function f_c for a given z and
    //                  a given c. Use the helper functions.
       var z_sqrd = mult(z,z);
       var f = add(z_sqrd,c); 
       return f;

}

function countIterations(start_z, c, max_iter) {
    // TODO 1.4a):      Count iterations needed for the sequence to diverge.
    //                  z is declared diverged as soon as its absolute value
    //                  exceeds 2. If the sequence does not diverge during
    //                  the first max_iter iterations, return max_iter. Use
    //                  function f_c().
    var z_old = start_z;
    var iter = 0;
    while ((abs(z_old) < 2) && (iter <= max_iter)){
        var z_new = new f_c(z_old,c);
        z_old = z_new;
        iter++;
    }
    if (abs(z_old) > 2){
     return iter;
    }
    else{
    return max_iter;
   }
    
    // TODO 1.4b):      Change the return value of this function to avoid
    //                  banding.



}


/////////////////////////////
/////////  Colors  //////////
/////////////////////////////
function getColorForIter(iter) {
    // find out which radio button is checked, i.e. which color scheme is picked
    var colorscheme;
    var radios = document.getElementsByName('colors');
    for (var i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            colorscheme = radios[i].value;
            break;
        }
    }

    // return color according to chosen color scheme
    var color = [128, 128, 128,128];
    if (colorscheme == "black & white") {
        // TODO 1.4a):      Return the correct color for the iteration count
        //                  stored in iter. Pixels corresponding to complex
        //                  numbers for which the sequence diverges should be
        //                  shaded white. Use the global variable max_iter.
          // if I get max_iter then give black 
        if ( iter != max_iter){
            color = [255 , 255 , 255 , 255];
        }
        else{
            color = [0,0,0,255];
        }
        


    } else if (colorscheme == "greyscale") {
        // TODO 1.4b):      Choose a greyscale color according to the given
        //                  iteration count in relation to the maximum
        //                  iteration count. The more iterations are needed
        //                  for divergence, the darker the color should be.
        //                  Be aware of integer division!
        if ( iter != max_iter){
            var temp_iter = (iter/max_iter);
            color = [Math.trunc(255*temp_iter) , Math.trunc(255*temp_iter)  , Math.trunc(255*temp_iter),255 ];
        }
        else{
            color = [0,0,0,255];
       }


    } else if (colorscheme == "underwater") {
        // TODO 1.4b):      Choose a color between blue and green according
        //                  to the given iteration count in relation to the
        //                  maximum iteration count. The more iterations are
        //                  needed for divergence, the more green and less
        //                  blue the color should be.
        if ( iter != max_iter){
            var temp_iter = (iter/max_iter);
            color = [13 , 152 , 186, Math.trunc(255*temp_iter)];
        }//
        else{
            color = [0,0,0,255];
        }
    

    } else { // rainbow
        // TODO 1.4b):      Choose a rainbow color according to the given
        //                  iteration count in relation to the maximum
        //                  iteration count. Colors should change from blue
        //                  (for very few needed iterations) over violet, pink,
        //                  red, yellow and green back to blue (for lots of
        //                  needed iterations). Use the HSV model and convert
        //                  HSV to RGB colors using function hsv2rgb.
        if ( iter != max_iter){
           var hsv = [240, 1, 1];
           
           switch (Math.trunc(iter / 6)) {
           case 1: hsv[0] = 270; break;
           case 1: hsv[0] = 330; break;
           case 2: hsv[0] = 0;break;
           case 2: hsv[0] = 60; break;
           case 3: hsv[0] = 120; break;
           case 3: hsv[0] = 240; break;
           }
           var color = hsv2rgb(hsv);
        }
        else{
            color = [0,0,0,255];
        }
    }

    return color;
}

function mix(a, b, v)
{
    return (1-v)*a + v*b;
}
function hsv2rgb(hsv) {
    var H = hsv[0];
    var S = hsv[1];
    var V = hsv[2];
    var rgb = [255, 255, 255 , 255];
    var V2 = V * (1 - S);
    var r  = ((H>=0 && H<=60) || (H>=300 && H<=360)) ? V : ((H>=120 && H<=240) ? V2 : ((H>=60 && H<=120) ? mix(V,V2,(H-60)/60) : ((H>=240 && H<=300) ? mix(V2,V,(H-240)/60) : 0)));
    var g  = (H>=60 && H<=180) ? V : ((H>=240 && H<=360) ? V2 : ((H>=0 && H<=60) ? mix(V2,V,H/60) : ((H>=180 && H<=240) ? mix(V,V2,(H-180)/60) : 0)));
    var b  = (H>=0 && H<=120) ? V2 : ((H>=180 && H<=300) ? V : ((H>=120 && H<=180) ? mix(V2,V,(H-120)/60) : ((H>=300 && H<=360) ? mix(V,V2,(H-300)/60) : 0)));
    rgb[0] =  Math.round(r * 255);
    rgb[1] =  Math.round(g * 255);
    rgb[2] =  Math.round(b * 255);
    // TODO 1.4b):      Replace the following line by code performing the
    //                  HSV to RGB convertion known from the lecture.
    return rgb;
}


/////////////////////////////////////
/////////  Canvas Fillers  //////////
/////////////////////////////////////
function mandelbrotSet(image) {
    for (var i = 0; i < 4 * image.width * image.height; i += 4) {
        var pixel = i / 4;
        var x = pixel % image.width;
        var y = image.height - pixel / image.width; 
        var c = new ComplexNumberFromCoords(x, y, 'mandelbrot_canvas'); 
        
        // TODO 1.4a):      Replace the following line by creation of the
        //                  Mandelbrot set. Use function countIterations().
       
        var iter = countIterations(new ComplexNumber(0,0), c, max_iter);
        var rgb = getColorForIter(iter);
        image.data[i] = rgb[0];
        image.data[i + 1] = rgb[1];
        image.data[i + 2] = rgb[2];
        image.data[i + 3] = rgb[3];
        }
    }

function juliaSet(image) {
    for (var i = 0; i < 4 * image.width * image.height; i += 4) {
        var pixel = i / 4;
        var x = pixel % image.width;
        var y = image.height - pixel / image.width;

        // TODO 1.4d):       Replace the following line by creation of the
        //                  Julia set for c = juliaC (global variable). Use
        //                  functions ComplexNumber(),
        //                  countIterations() and getColorForIter().
        var rgb = [128, 128, 128];
        image.data[i] = rgb[0];
        image.data[i + 1] = rgb[1];
        image.data[i + 2] = rgb[2];
        image.data[i + 3] = 255;
    }
}

///////////////////////////////
/////////  Renderers  //////////
///////////////////////////////
function RenderMandelbrotSet() {
    // get the canvas
    var canvas = document.getElementById("mandelbrot_canvas");
    c = canvas.getContext("2d");

    // create a new image
    image = c.createImageData(canvas.width, canvas.height);

    // render Mandelbrot set
    mandelbrotSet(image);

    // write image back to canvas
    c.putImageData(image, 0, 0);
}

function RenderJuliaSet() {
    // get the canvas
    var canvas = document.getElementById("julia_canvas");
    c = canvas.getContext("2d");

    // create a new image
    image = c.createImageData(canvas.width, canvas.height);

    // render Julia set
    juliaSet(image);

    // write image back to canvas
    c.putImageData(image, 0, 0);
}


///////////////////////////////
//////////   "main"   /////////
///////////////////////////////

// maximum iteration number for Mandelbrot computation
var max_iter = 30;

// coordinate system center
var center = new ComplexNumber(-0.5, 0);

// zoom stage
var zoom = 0;

// flag to show if mouse is pressed
var dragging = false;

// helper variable for moving around
var lastPoint;

// c for Julia set creation
var juliaC = new ComplexNumber(0.4, 0.1);

function setupMandelbrot(canvas) {
    // reset color scheme and maximum iteration number
    var radios = document.getElementsByName('colors');
    radios[0].checked = true;
    var slider = document.getElementById('slider');
    slider.value = 30;

    // render
    RenderMandelbrotSet();
    RenderJuliaSet();

    // add event listeners
    canvas.addEventListener('mousedown', onMouseDown, false);
    canvas.addEventListener('mousemove', onMouseMove, false);
    canvas.addEventListener('mouseup', onMouseUp, false);

    // TODO 1.4c):      Uncomment the following line to enable zooming.
    //canvas.addEventListener('DOMMouseScroll', onMouseWheel, false);
}


//////////////////////////////////////
//////////   Event Listeners   ///////
//////////////////////////////////////
function onMouseDown(e) {
    var canvas = document.getElementById("mandelbrot_canvas");
    var rect = canvas.getBoundingClientRect();
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;
    y = canvas.height - y;

    if (e.ctrlKey) {
        // choose new c for Julia set creation
        juliaC = new ComplexNumberFromCoords(x, y, 'mandelbrot_canvas');
        RenderJuliaSet();
    } else {
        // TODO 1.4c):      Store the hit point as pixel coordinates and
        //                  start the dragging process. Use the global
        //                  variables dragging (bool) and lastPoint (two
        //                  dimensional vector).



    }
}

function onMouseMove(e) {
    if (dragging) {
        var canvas = document.getElementById("mandelbrot_canvas");
        var rect = canvas.getBoundingClientRect();
        var x = e.clientX - rect.left;
        var y = e.clientY - rect.top;
        y = canvas.height - y;

        // TODO 1.4c):      Convert both last and current hit point to
        //                  their corresponding complex numbers, compute
        //                  their distance (also as a complex number) and
        //                  shift the plane accordingly. To do so, change
        //                  the global variable center which is used to
        //                  compute the complex number corresponding to a pixel.



        // rerender image
        RenderMandelbrotSet();
    }
}

function onMouseUp(e) {
    // TODO 1.4c):      Prevent dragging of the plane once the mouse is
    //                  not pressed anymore.



}

function onMouseWheel(e) {
    var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
    zoom = zoom + delta;

    // render
    RenderMandelbrotSet();

    // do not scroll the page
    e.preventDefault();
}

function onChangeMaxIter(value) {
    max_iter = value;

    // render
    RenderMandelbrotSet();
    RenderJuliaSet();
}

function onChangeColorScheme() {
    // render
    RenderMandelbrotSet();
    RenderJuliaSet();
}
