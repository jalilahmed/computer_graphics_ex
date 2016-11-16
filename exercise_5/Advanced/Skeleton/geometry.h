#pragma once

#include "helper.h"

//  C++ stuff 
#define _USE_MATH_DEFINES 
#include <cmath>
#include <math.h>
#include <vector>

//glm stuff

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


// OpenGL stuff
#include "gl/glew.h" 

using namespace glm;
using namespace std;

struct Vertex {


	Vertex(const vec3& p) :position(p) { };

	vec3 position;
};

class Geometry
{
public:

	virtual void create() = 0;


	virtual void bindVertexArrayObjects(const vector<Vertex> &vertexdata, const vector<unsigned short> &indices)
	{

		//*bind to GL
		glCreateVertexArrays(1, &(vao));

		glBindVertexArray(vao);

		numVertices = vertexdata.size();
		numIndices = indices.size();

		// Create and bind a BO for vertex data
		GLuint vbo;
		glCreateBuffers(1, &vbo);

		glNamedBufferData(vbo, numVertices * sizeof(Vertex), vertexdata.data(), GL_STATIC_DRAW);

		// set up vertex attributes


		glEnableVertexArrayAttrib(vao,	// VAO name.
			0);
		//
		glVertexArrayAttribFormat(vao,	// VAO name.
			0,							// Attribute index (layout in shader).
			3,							// Component count (x,y,z).
			GL_FLOAT,					// Type.
			GL_FALSE,					// Normalised.
			0);							// Offset (bytes).
										//
		glVertexArrayVertexBuffer(vao,    // VAO name.
			0,										// Binding point.
			vbo,									// VBO name.
			0,										// Offset (bytes).
			sizeof(Vertex));						// Stride (bytes). THIS FUCKER IS SWITCHED WTF???
													//
													//
													//
		glVertexArrayAttribBinding(vao,   // VAO name.
			0,										// Attribute index (layout in shader).
			0);									// Binding point.
												// Attribute index (layout in shader).

												// Create and bind a BO for index data
		GLuint ibo;
		glCreateBuffers(1, &ibo);

		// copy data into the buffer object
		glNamedBufferData(ibo, numIndices * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

		glVertexArrayElementBuffer(vao, ibo);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		GL_CHECK_ERROR("bindVertexArrayObjects");


	}



	virtual	void draw(int instanceCount = 1)
	{

		glBindVertexArray(vao);
		glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0, instanceCount);
		glBindVertexArray(0);
	}

	GLuint vao;

	unsigned int numVertices;
	unsigned int numIndices;

};

class GSphere : public Geometry
{
public:
	void create()
	{
		float r = 1;
		int slices = 12;
		int stacks = 15;

		float dTheta = 2.0*M_PI / (float)stacks;
		float dPhi = M_PI / (float)slices;

		vector<Vertex> vertexdata;
		vector<unsigned short> indices;

		vec2 t(0.5, 1.0);
		vec3 n(0, 1, 0);
		vec3 p(0, r, 0);
		vertexdata.push_back(Vertex(p));

		//North pole
		for (int i = stacks; i >= 0; i--) {
			vec2 t(1 - i*dTheta / (2.0*M_PI), (M_PI - dPhi) / M_PI);
			vec3 n(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
			vec3 p(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
			vertexdata.push_back(Vertex(p));
		}
		int triangleID = 0;
		for (; triangleID < stacks; triangleID++)
		{
			indices.push_back(0);
			indices.push_back(triangleID);
			indices.push_back(triangleID + 1);
		}

		indices.push_back(0);
		indices.push_back(triangleID);
		indices.push_back(1);


		// Middle Part 

		//	v0--- v2
		//  |  	/ |
		//  |  /  | 
		//  | /   |
		//  v1--- v3

		for (int j = 1; j < slices - 1; j++)
		{
			for (int i = stacks; i >= 0; i--)
			{
				t = vec2(1 - i*dTheta / (2.0*M_PI), (M_PI - (j + 1)*dPhi) / M_PI);
				n = vec3(sin((j + 1)*dPhi)*cos(i*dTheta), cos((j + 1)*dPhi), sin((j + 1)*dPhi)*sin(i*dTheta));
				p = vec3(r*sin((j + 1)*dPhi)*cos(i*dTheta), r*cos((j + 1)*dPhi), r*sin((j + 1)*dPhi)*sin(i*dTheta));
				vertexdata.push_back(Vertex(p));

				//add two triangles

				indices.push_back(vertexdata.size() - stacks - 1);
				indices.push_back(vertexdata.size() - stacks - 2);		 
				indices.push_back(vertexdata.size() - 1);				 
																		 
				indices.push_back(vertexdata.size() - stacks - 1);		 
				indices.push_back(vertexdata.size() - 1);				 
				indices.push_back(vertexdata.size());					 

			}

		}

		//South pole
		t = vec2(0.5, 0);
		n = vec3(0, -1, 0);
		p = vec3(0, -r, 0);
		vertexdata.push_back(Vertex(p));

		int lastVertex = vertexdata.size() - 1;

		triangleID = 0;
		for (; triangleID <= stacks; triangleID++)
		{
			indices.push_back(lastVertex);
			indices.push_back(vertexdata.size() - triangleID - 1);
			indices.push_back(vertexdata.size() - triangleID - 2);
		}


		bindVertexArrayObjects(vertexdata, indices);

	}

};

class GRings : public Geometry
{
public:
	void create()
	{
		float r = 1;
		int slices = 24;


		float dr = .3;
		float dPhi = 2.0*M_PI / (float)(slices);

		vector<Vertex> vertexdata;
		vector<unsigned short> indices;

		// Middle Part 

		//	v0--- v2
		//  |  	/ |
		//  |  /  | 
		//  | /   |
		//  v1--- v3

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < slices; j++)
			{
				vec2 t = vec2(1 - j*dPhi / (2.0*M_PI), i);
				vec3 n = vec3(0, 1, 0);

				vec3 p = glm::rotate(glm::angleAxis(dPhi*j, vec3(0, 1, 0)), vec3(0, 0, r + i*dr));
				//cout <<"P: "<< p.x << " " << p.y << " " << p.z << " " << endl;
				vertexdata.push_back(Vertex(p)); 

				//add two triangles
				if (j > 0 && i > 0)
				{
					int i0 = vertexdata.size() - 2;
					int i1 = vertexdata.size() - slices - 1;
					int i2 = vertexdata.size() - slices - 2;
					int i3 = vertexdata.size() - 1;

					//cout << "T: " << i0 << " " << i1 << " " << i2 << " " << endl;


			 	 	indices.push_back(i0);		 
			 	 	indices.push_back(i1);		 
			 	 	indices.push_back(i2);		 
												 
					indices.push_back(i3);		 
					indices.push_back(i1);		 
					indices.push_back(i0);	


					indices.push_back(i0);
					indices.push_back(i2);
					indices.push_back(i1);

					indices.push_back(i3);
					indices.push_back(i0);
					indices.push_back(i1);

				}
			}
			cout << endl;
		}
		int i0 = vertexdata.size() - 1   ;
		int i1 = vertexdata.size() - 2*slices;
		int i2 = vertexdata.size() - slices-1;
		int i3 = vertexdata.size() - slices;

		//cout << "T: " << i0 << " " << i1 << " " << i2 << " " << endl;


		indices.push_back(i0);
		indices.push_back(i1);
		indices.push_back(i2);

		indices.push_back(i3);
		indices.push_back(i1);
		indices.push_back(i0);

		indices.push_back(i0);
		indices.push_back(i2);
		indices.push_back(i1);

		indices.push_back(i3);
		indices.push_back(i0);
		indices.push_back(i1);


		bindVertexArrayObjects(vertexdata, indices);

	}
	 
};
