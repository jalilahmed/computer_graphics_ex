function drawPixelwiseCircle(canvas) {
  var context = canvas.getContext("2d");
  var img = context.createImageData(200, 200);
  //TODO 1.1a)      Copy the code from Basic.js
  //                and modify it to create a 
  //                circle.
  var radius = 50;
	for (var x = 0 ; x <  200 ; x++ ) {
	  for (var y = 0 ; y <   200 ; y++){
	    if ( Math.pow((x - 100),2) + Math.pow((y - 100), 2) < Math.pow(radius , 2)){
			img.data[4 *(y * img.width + x)] = 00;
			img.data[4 *(y * img.width + x) + 1] = 255;
			img.data[4 * (y * img.width + x) + 2] = 00;
			img.data[4 * (y * img.width + x) + 3] = 255;
	      }
	    }
	}
  context.putImageData(img, 0, 0);
}
function drawContourCircle(canvas) {
  var context = canvas.getContext("2d");
  var img = context.createImageData(200, 200);
  //TODO 1.1b)      Copy your code from above
  //                and extend it to receive a
  //                contour around the circle.
 var radius = 50;
 var radius2 = 60;
	for (var x = 0 ; x <  200 ; x++ ) {
	  for (var y = 0 ; y <   200 ; y++){
	    if ( Math.pow((x - 100),2) + Math.pow((y - 100), 2) < Math.pow(radius , 2)){
			img.data[4 * (y * img.width + x)] = 0;
			img.data[4 * (y * img.width + x) + 1] = 255;
			img.data[4 * (y * img.width + x) + 2] = 0;
			img.data[4 * (y * img.width + x) + 3] = 255;
	      }
	    if ( Math.pow(radius, 2) <  Math.pow((x - 100),2) + Math.pow((y - 100), 2) && Math.pow((x - 100),2) + Math.pow((y - 100), 2) < Math.pow(radius2, 2)){	
			img.data[4 * (y * img.width + x)] = 0;
			img.data[4 * (y * img.width + x) + 1] = 102;
			img.data[4 * (y * img.width + x) + 2] = 0;
			img.data[4 * (y * img.width + x) + 3] = 255;
	    }
	  }
	}
  context.putImageData(img, 0, 0);
}
function drawSmoothCircle(canvas) {
  var context = canvas.getContext("2d");
  var img = context.createImageData(200, 200);

  //TODO 1.1c)      Copy your code from above
  //                and extend it to get rid
  //                of the aliasing effects at
  //                the border.
	var radius = 60;
	for (var x = 0 ; x <  200 ; x++ ) {
	  for (var y = 0 ; y < 200 ; y++){
	      // if ((Math.pow((x - 100),2) + Math.pow((y - 100), 2)) < Math.pow((radius - 10), 2)){
			//img.data[4 * (y * img.width + x)] = 0;
			//img.data[4 * (y * img.width + x) + 1] = 255;
			//img.data[4 * (y * img.width + x) + 2] = 0;
			//img.data[4 * (y * img.width + x) + 3] = 255;
			//	}
			// Math.pow(radius - 10, 2) <  Math.pow((x - 100),2) + Math.pow((y - 100), 2) &&
	     if ((Math.pow((x - 100),2) + Math.pow((y - 100), 2)) < Math.pow(radius, 2)){
								if ((Math.pow((x - 100),2) + Math.pow((y - 100), 2)) < Math.pow((radius - 10), 2)){
									img.data[4 * (y * img.width + x)] = 0;
									img.data[4 * (y * img.width + x) + 1] = 255;
									img.data[4 * (y * img.width + x) + 2] = 0;
									img.data[4 * (y * img.width + x) + 3] = 255;
									}else{
				                var distance = (Math.pow((x - 100),2) + Math.pow((y - 100), 2))/Math.pow(radius,2);
								        var value = (255-102)*distance;
								        img.data[4 * (y * img.width + x)] =  0;
								        img.data[4 * (y * img.width + x) + 1] = 255-value;
								        img.data[4 * (y * img.width + x) + 2] = 0;
								        img.data[4 * (y * img.width + x) + 3] = 255;
									     }
	              }
	  }
	}
  context.putImageData(img, 0, 0);
}
