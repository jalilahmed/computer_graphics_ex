function drawArcCircle(canvas) {
  var context = canvas.getContext("2d");

  //TODO 1.2)       Use the arc function to
  //                rasterize the two circles
  //                from Task 1.1.
  context.beginPath();
  context.arc(50,100,45,0,2*Math.PI);
  context.stroke();
  context.fillStyle = "#00FF00";
  context.fill();
  context.beginPath(); //If I donnot write it here then the two circle are connected. 
  context.arc(150,100,50,0,2*Math.PI);
  context.stroke();
  context.fillStyle = "#006600";
  context.fill();
  context.beginPath(); //If I donnot write it here then the two circle are connected. 
  context.arc(150,100,45,0,2*Math.PI);
  context.stroke();
  context.fillStyle = "#00FF00";
  context.fill();
 
  
  

}
