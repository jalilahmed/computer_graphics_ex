/////////////////////////////
//////////   helper   ///////
/////////////////////////////
function Point(x, y) {
    this.x = x;
    this.y = y;
}

function Triangle(pointA, pointB, pointC) {
    this.a = pointA;
    this.b = pointB;
    this.c = pointC;
}

function Viewport(width, height, x, y) {
    this.width = width;
    this.height = height;
    this.x = x;
    this.y = y;
}

function RenderTriangle(context, viewport, triangle, clear) {
    if (clear == undefined) clear = true;
    if (clear) {
        context.rect(viewport.x, viewport.y, viewport.width, viewport.height);
        context.stroke();
    }

    context.beginPath();
    context.moveTo(viewport.width * triangle.a.x + viewport.x, viewport.height * triangle.a.y + viewport.y);
    context.lineTo(viewport.width * triangle.b.x + viewport.x, viewport.height * triangle.b.y + viewport.y);
    context.lineTo(viewport.width * triangle.c.x + viewport.x, viewport.height * triangle.c.y + viewport.y);
    context.lineTo(viewport.width * triangle.a.x + viewport.x, viewport.height * triangle.a.y + viewport.y);
    context.fill();
}

function LinearTransformation(linearPart) {
    this.A = linearPart;
}

function ApplyLinearTransformation(linearTransf, point) {
    return new Point(linearTransf.A[0] * point.x + linearTransf.A[1] * point.y,
                        linearTransf.A[2] * point.x + linearTransf.A[3] * point.y)
}

function CompositeLinearTransformations(linearTransf2, linearTransf1)
{
    return new LinearTransformation([linearTransf2.A[0] * linearTransf1.A[0] + linearTransf2.A[1] * linearTransf1.A[2], linearTransf2.A[0] * linearTransf1.A[1] + linearTransf2.A[1] * linearTransf1.A[3],
                                    linearTransf2.A[2] * linearTransf1.A[0] + linearTransf2.A[3] * linearTransf1.A[2], linearTransf2.A[2] * linearTransf1.A[1] + linearTransf2.A[3] * linearTransf1.A[3]]);
}

function AffineTransformation(linearPart, translPart) {
    this.A = linearPart;
    this.t = translPart;
}

function ApplyAffineTransformation(affineTransf, point) {
    return new Point(affineTransf.A[0] * point.x + affineTransf.A[1] * point.y + affineTransf.t[0],
                        affineTransf.A[2] * point.x + affineTransf.A[3] * point.y + affineTransf.t[1])
}

var Basic1_1 = function () {

    function Rotation(alpha) {
        // :	Implement a linear transformation 
		//			performing a rotation by the angle 
		//			alpha and replace the following line
		//			by the appropriate code.
		// 			We recieve the angle from the main function
		return new LinearTransformation([Math.cos(alpha), -(Math.sin(alpha)), Math.sin(alpha), Math.cos(alpha)]);
    }

    function Scaling(scale) {
        // :	Implement a linear transformation 
		//			performing an isotropic scaling by 
		//			the scaling factor scale and replace
		//			the following line by the appropriate 
		//			code.
		return new LinearTransformation([scale, 0, 0, scale]);
    }

    function ShearingX(shearX) {
        // :	Implement a linear transformation 
		//			performing a shear along the x axis. 
		//			Replace the following line by the
		//			appropriate code.
		return new LinearTransformation([1, shearX, 0, 1]);
    }

    return {
        start: function (canvas) {
            var context = canvas.getContext("2d");
            context.clearRect(0, 0, 600, 150);
            context.font = "18px Georgia";
            context.textAlign = "center";

            context.fillText("Input Triangle", 75, 140);
            var triangle = new Triangle(new Point(0.2, 0.2), new Point(0.8, 0.2), new Point(0.2, 0.8));
            RenderTriangle(context, new Viewport(150, 150, 0, 0), triangle);

            context.fillText("Rotated Triangle", 225, 140);
            var rot = Rotation(0.2);
            var triangleRot = new Triangle(ApplyLinearTransformation(rot, triangle.a),
                                            ApplyLinearTransformation(rot, triangle.b),
                                            ApplyLinearTransformation(rot, triangle.c));
            RenderTriangle(context, new Viewport(150, 150, 150, 0), triangleRot);

            context.fillText("Scaled Triangle", 375, 140);
            var scaling = Scaling(0.5);
            var triangleScaling = new Triangle(ApplyLinearTransformation(scaling, triangle.a),
                                                ApplyLinearTransformation(scaling, triangle.b),
                                                ApplyLinearTransformation(scaling, triangle.c));
            RenderTriangle(context, new Viewport(150, 150, 300, 0), triangleScaling);

            context.fillText("Sheared Triangle", 525, 140);
            var shearing = ShearingX(0.4);
            var triangleShearing = new Triangle(ApplyLinearTransformation(shearing, triangle.a),
                                                ApplyLinearTransformation(shearing, triangle.b),
                                                ApplyLinearTransformation(shearing, triangle.c));
            RenderTriangle(context, new Viewport(150, 150, 450, 0), triangleShearing);
        }
    }
}()

var Basic1_2 = function () {

    function ShearingX(shearX) {
        // :	Implement a linear transformation 
		//			performing a shear along the x axis. 
	return new LinearTransformation([1, -(Math.tan(shearX/2)), 0 , 1]);
    }

    function ShearingY(shearY) {
        // :	Implement a linear transformation 
		//			performing a shear along the y axis. 
	return new LinearTransformation([1, 0, Math.sin(shearY) , 1]);    
	}

    return {
        start: function (canvas) {
            var context = canvas.getContext("2d");
            context.clearRect(0, 0, 600, 150);
            context.font = "18px Georgia";
            context.textAlign = "center";

            context.fillText("Input Triangle", 75, 140);
            var triangle = new Triangle(new Point(0.2, 0.2), new Point(0.8, 0.2), new Point(0.2, 0.8));
            RenderTriangle(context, new Viewport(150, 150, 0, 0), triangle);

            var alpha = 0.2;

            context.fillText("1. Shearing", 225, 140);
            //:	Instead of just copying the corner points
			//of triangle, call shearingX with the 
            //			corresponding parameters!
            //          Use ApplyLinearTransformation() to transform the corner points.
	
			//var triangle1 = new Triangle(triangle.a, triangle.b, triangle.c);
			var shearX = ShearingX(alpha);
			var triangle1 = new Triangle(ApplyLinearTransformation(shearX,triangle.a), 
								ApplyLinearTransformation(shearX,triangle.b),
								ApplyLinearTransformation(shearX,triangle.c));
			RenderTriangle(context, new Viewport(150, 150, 150, 0), triangle1);

            context.fillText("2. Shearing", 375, 140);
            //:	Instead of just copying the corner points
			//			of triangle1, call shearingY with the 
            //			corresponding parameters!
            //          Use ApplyLinearTransformation() to transform the corner points.
			//var triangle2 = new Triangle(triangle1.a, triangle1.b, triangle1.c);
			var shearY = ShearingY(alpha);
			var triangle2  = new Triangle(ApplyLinearTransformation(shearY, triangle1.a),
								ApplyLinearTransformation(shearY, triangle1.b),
								ApplyLinearTransformation(shearY, triangle1.c));
			RenderTriangle(context, new Viewport(150, 150, 300, 0), triangle2);

            context.fillText("3. Shearing", 525, 140);
            //:	Instead of just copying the corner points
			//			of triangle2, call shearingX with the 
            //			corresponding parameters!
            //          Use ApplyLinearTransformation() to transform the corner points.
			//var triangle3 = new Triangle(triangle2.a, triangle2.b, triangle2.c);
			var triangle3  = new Triangle(ApplyLinearTransformation(shearX, triangle2.a),
								ApplyLinearTransformation(shearX, triangle2.b),
								ApplyLinearTransformation(shearX, triangle2.c));
			RenderTriangle(context, new Viewport(150, 150, 450, 0), triangle3);
        }
    }
}()

var Basic1_3 = function () {

    function CompositeAffineTransformations(affineTransf2, affineTransf1) {
        // :	Replace the following line by creation
	//		of the affine transformation equivalent
	//		to the composition of affineTransf1 and
	//		affineTransf2.
	var B = affineTransf1.A;
	//console.log(B);
	var b = affineTransf1.t;
	var C = affineTransf2.A;
	//console.log(C);
	var c = affineTransf2.t;
	var composite_A = [(C[0]*B[0] + C[1]*B[2]),(C[0]*B[1] + C[1]*B[3]),(C[2]*B[0] + C[3]*B[2]),(C[2]*B[1] + C[3]*B[3])];
	//console.log(composite_A);
	var composite_t = [(C[0]*b[0] + C[1]*b[1] + c[0]),(C[2]*b[0] + C[3]*b[1] + c[1])];
	//console.log(composite_t);
        return new AffineTransformation(composite_A, composite_t);
    }

    return {
        start: function (canvas) {
            var context = canvas.getContext("2d");
            context.clearRect(0, 0, 600, 150);
            context.font = "18px Georgia";
            context.textAlign = "center";

            var affineTransformation1 = new AffineTransformation([Math.cos(Math.PI/12), -Math.sin(Math.PI/12), Math.sin(Math.PI/12), Math.cos(Math.PI/12)], [0.3, 0.0]);
            var affineTransformation2 = new AffineTransformation([Math.cos(-Math.PI/8), -Math.sin(-Math.PI/8), Math.sin(-Math.PI/8), Math.cos(-Math.PI/8)], [0.0, 0.1]);
            var affineTransformation3 = CompositeAffineTransformations(affineTransformation2, affineTransformation1);

            context.fillText("Input Triangle", 75, 140);
            var triangle = new Triangle(new Point(0.05, 0.2), new Point(0.65, 0.2), new Point(0.05, 0.8));
            RenderTriangle(context, new Viewport(150, 150, 0, 0), triangle);

            context.fillText("1. Transf.", 225, 140);
            var triangle1 = new Triangle(ApplyAffineTransformation(affineTransformation1, triangle.a), ApplyAffineTransformation(affineTransformation1, triangle.b), ApplyAffineTransformation(affineTransformation1, triangle.c));
            RenderTriangle(context, new Viewport(150, 150, 150, 0), triangle1);

            context.fillText("1. then 2. Transf.", 375, 140);
            var triangle2 = new Triangle(ApplyAffineTransformation(affineTransformation2, triangle1.a), ApplyAffineTransformation(affineTransformation2, triangle1.b), ApplyAffineTransformation(affineTransformation2, triangle1.c));
            RenderTriangle(context, new Viewport(150, 150, 300, 0), triangle2);

            context.fillText("Composite Transf.", 525, 140);
            var triangle3 = new Triangle(ApplyAffineTransformation(affineTransformation3, triangle.a), ApplyAffineTransformation(affineTransformation3, triangle.b), ApplyAffineTransformation(affineTransformation3, triangle.c));
            RenderTriangle(context, new Viewport(150, 150, 450, 0), triangle3);
        }
    }
}()

var Basic1_4 = function () {
    function ComputeMapping(triangleTarget) {
        // Note: The original triangle has the fixed vertices (0, 0), (1, 0) and (0, 1).
	// DID MATHEMATICAL CALLCULATION HERE
        // : 	Compute the affine transformation that 
		//			transforms the triangle to the target 
		//			triangle triangleTarget!
		var t = [triangleTarget.a.x,triangleTarget.a.y];
		var a = triangleTarget.b.x - t[0];
		var c = triangleTarget.b.y - t[1];
		var b = triangleTarget.c.x - t[0];
		var d = triangleTarget.c.y - t[1];
		
        return new AffineTransformation([a, b,c,d], t);

    }

    return{
        start : function(canvas) {
            var context = canvas.getContext("2d");
            context.clearRect(0, 0, 600, 200);
            context.font = "18px Georgia";
            context.textAlign = "center";

            context.fillText("Input Triangle", 100, 180);
            var triangle = new Triangle(new Point(0, 0), new Point(1, 0), new Point(0, 1));
            RenderTriangle(context, new Viewport(200, 200, 0, 0), triangle);

            context.fillText("Target Triangle", 500, 180);
            var triangleTarget = new Triangle(new Point(0.2, 0.2), new Point(0.8, 0.0), new Point(0.1, 0.9));
            RenderTriangle(context, new Viewport(200, 200, 400, 0), triangleTarget);

            context.fillText("Mapped Triangle", 300, 180);
            var affineTransf = ComputeMapping(triangleTarget);
            var triangleTransformed = new Triangle(ApplyAffineTransformation(affineTransf, triangle.a), ApplyAffineTransformation(affineTransf, triangle.b), ApplyAffineTransformation(affineTransf, triangle.c));
            RenderTriangle(context, new Viewport(200, 200, 200, 0), triangleTransformed);
        }
    }
}()
