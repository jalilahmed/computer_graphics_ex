#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <math.h>

//glm stuff

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;
using namespace std;

class Camera
{

public:



	mat4 V() {

		return mat4_cast(rotation) * translate(position);
	};
	mat4 P() { return proj; };

	void fromLookatVector(vec3 _eye, vec3  _focus, vec3  _up)
	{


		rotation = normalize(quat(lookAt(_eye, _focus, _up)));
		position = -_eye;

	}

	void setP(const mat4&_proj) { proj = _proj; }



	void updateTransformation(float deltaT)
	{


		// Camera Rotation
		{
			quat dAngularAcceleration = quat();
			if (length(inputRotationAxis) != 0)
			{

				dAngularAcceleration = normalize(angleAxis(float(angularAcceleration / M_PI*deltaT), inputRotationAxis));
				inputRotationAxis = vec3(0);
			}

			float angleSpeed = angle(angularVelocity);
			vec3 angleaxis = axis(angularVelocity);


			angularVelocity = angleAxis(.9f*clamp(angleSpeed, -anglespeedmax, anglespeedmax), angleaxis);
			angularVelocity = normalize(dAngularAcceleration*angularVelocity);

			 
			rotation = normalize(angularVelocity * rotation);
		}


		// Camera Translation
		{
			float a = length(inputTranslationVector);

			vec3 dLinearAcceleration = vec3(0);
			if (a != 0)
				dLinearAcceleration = glm::rotate(inverse(rotation), normalize(inputTranslationVector)) * linearAcceleration;


			linearVelocity += dLinearAcceleration ;

			if (length(linearVelocity) > speedMax)
				linearVelocity = speedMax*normalize(linearVelocity);

			 if (a == 0)
			 	linearVelocity *= .9;

			// clamping maps entire quadrants to corner	
			//	linearVelocity = .9f*clamp(linearVelocity + dLinearAcceleration, -linearVelocityMax, linearVelocityMax);

			if (length(linearVelocity) < .01)
				linearVelocity = vec3(0);

			position += linearVelocity;

			inputTranslationVector = vec3(0);
		}


	};

	void accelerate(vec3 _dPos)
	{
		inputTranslationVector += _dPos;
	}


	void changeSpeedLimit(float s)
	{
		speedMax = glm::max(0.f, speedMax * (1 + .3f*s));
		anglespeedmax = glm::max(0.f, anglespeedmax * (1 + .3f*s));
		cout << "speedMax: " << speedMax << endl;
	}

	void rotateCamera(vec3 axis)
	{
		inputRotationAxis += axis;

	}


	void setTransform(const vec3& _position, const quat& _rotation)
	{
		position = _position;
		rotation = _rotation;
	}
private:
	vec3 position;
	quat rotation;
	mat4   proj;



	vec3 inputTranslationVector;
	vec3 inputRotationAxis;

	const float angularAcceleration = .08;
	quat angularVelocity;


	const float linearAcceleration = .7;
	vec3  linearVelocity = vec3(0);

	float speedMax = 3;
	float anglespeedmax = 0.2f / M_PI;



};

