

#define GLM_FORCE_RADIANS
#include "helper.h"
#include "camera.h"
#include "geometry.h"

//  C++ stuff
#define _USE_MATH_DEFINES  
#include <cmath>
#include <math.h>
#include <chrono> 
#include <functional>
#include <vector>

//glm stuff

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


//sdl stuff

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>  

using namespace std;
using namespace glm;
  

//global declaration for event handling
bool wireframe_mode = false;
bool running = true;
Camera* cam; 

// globalöly declared shader and shader variables (uniforms)

GLuint  phongShader;  

GLuint phongShaderConstDataBuffer;
struct PhongShaderConstData
{
	mat4 M;
	mat4 VP;
	vec4 color;
} phongShaderConstData;
 
 
// every Object has a material, we will extend this later on
class Material
{
public:
	Material(vec4 _color)
	{
		color = _color;
	}

	vec4 color;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SceneNode: this is the scene graph
// Every node has children that are called recursively
// The core functionality is the combination of transformations that are applied from parent to child
//
// A SceneNode CAN have renderable geometry it is not a requirement
//
// The transformation of each node must be defined in an animate function to allow for utmost flexibilty
// This is OUR CHOICE, it is not canon	
//
// We  have divided the drawing from the transformations. 
// This has no immediate reason but allows  for multiple render passes with the same transformation.
// We will use this in some later exercise on blending
//
// The transformation is stored using quaternions and translations from which we compute the Model matrix
// This is redundant and  more for demonstration purposes.
// There arew advanced drawing modes that allow for a batched draw call where we have to upload ALL transformation data
// prior to drawing and rendering the scene using one draw call only.
// (check multidrawindirect if you are interested and "AZDO", also Vulkan).
//
//
// A SceneNode can have a camera attached to it. In which case the camera follows the transformation dictated by the scene graph
// 
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SceneNode
{
	public:
		SceneNode(Geometry* _geometry = 0, const vec3& _position = vec3(0), const quat& _rotation = quat(), float _scale = 1)
		{
			position = _position;
			rotation = _rotation;
			scale = _scale;
			geometry = _geometry;
		}

		void attachChild(SceneNode* child)
		{
			children.push_back(child);
		};
		
		// hook Camera into scenegraph

		void updateTransformation(mat4 parentM, float deltaT)
		{
			
			 if (animate)
			 {
			 	animate(deltaT, position, rotation);
			 }
			 else
			 {
			 	//cout << "No animation Defined" << endl;
			 }
		 	
			M = parentM * translate(position)  * mat4_cast(rotation);   
			
			if (cam != 0)
			{

				//TODO: compute camera transformation 
			 	//auto q ;
			 	rotation = inverse(rotation);
			 	//auto position ;  
				
			
				cam->setTransform(position, rotation);
				  
			}

			for (auto child : children)
			{
				child->updateTransformation(M, deltaT);
			}
		}
		void attachCam(Camera*  _cam)
		{
			cam = _cam;
		}
		 

		void draw()
		{
			phongShaderConstData.M = M * glm::scale(scale, scale, scale);
			phongShaderConstData.color = (material != 0) ? material->color : vec4(1,0,0,1);
			glNamedBufferSubData(phongShaderConstDataBuffer, 0, sizeof(phongShaderConstData), &phongShaderConstData);

			if (geometry != 0)
			{
				geometry->draw();
			}
			for (auto child : children)
			{
				child->draw();
			}
		}

		function<void(float, glm::vec3&, glm::quat&)>  animate;
		Material* material = 0;

	private:
	
		  
		float scale;
		quat rotation;
		vec3 position;


		mat4 M;
		vector<SceneNode*> children;
		Geometry* geometry = 0;
		Camera*  cam = 0;



};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene:  Everything is generally stored in a scene object
//
//	We are a little lax and store something globally.
//  Encapsulating everything in a nice and object oriented manner will quickly complicate things.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Scene
{
	public:
		Scene(SceneNode* _root, Camera * _cam) : root(_root), cam(_cam) {};

		void updateTransformation(float deltaT)
		{
			cam->updateTransformation(deltaT);
			root->updateTransformation(mat4(), deltaT);
		}

		void draw()
		{
			phongShaderConstData.VP = cam->P() * cam->V();
			root->draw();
		}

		SceneNode* root;
		Camera * cam;


};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initGL: here we inialize OpenGL specific stuff
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void initGL() {
	glClearDepth(1);
	glClearColor(1, 1, 1, 1);   // set the clear color to white
	glDisable(GL_DEPTH_TEST); // turn on the depth test
	glEnable(GL_CULL_FACE);  // turn on backface culling
	 
	 

	// create the shaders (the functions are defined in helper.h)

	 
	 createProgram_VF("shader/phong_VS.glsl", "shader/phong_FS.glsl", &phongShader, true);
 

	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 // UNIFORM_BUFFER_OBJECTS: are a nice way to group uniform variables
	 // we have to duplicate the structs on C++ and GLSL side
	 //
	 //
	 //
	 //
	 //
	 //
	 // the struct members MUST HAVE AN ALIGNMENT OF FLOAT 4 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 //	
	 //	!!! never forget that 
	 //
	 //
	 //
	 //
	 //	glNamedBufferData: new OpenGL syntax allows us to use buffer directly. Do not mix named buffers with glGenBuffers
	 //	
	 //
	 //
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 glCreateBuffers(1, &phongShaderConstDataBuffer);
	 glNamedBufferData(phongShaderConstDataBuffer,  sizeof(phongShaderConstData) , &phongShaderConstData, GL_STATIC_DRAW);
 




}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	handle_keyevent: SDL input handling
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
 

void handle_keyevent(const SDL_Keycode& keycode) {

	switch (keycode) { 

	case SDLK_ESCAPE: 
		running = false;
		break;
	case SDLK_p:
		wireframe_mode = !wireframe_mode;
		cout << "Switch Wireframe Mode: "<< (wireframe_mode?"on":"off") << endl;
		if (wireframe_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;


	default:;
	}
	
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	fastUpdate: SDL input handling
//
//	the SDL key polling is too rigid, this is a hack
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void fastUpdate()
{
	auto key = SDL_GetKeyboardState(NULL);


	if (key[ SDL_SCANCODE_W])
		cam->accelerate(vec3(0, 0, 1));

	if (key[SDL_SCANCODE_S])
		cam->accelerate(vec3(0, 0, -1));

	if (key[ SDL_SCANCODE_A])
		cam->accelerate(vec3(1, 0, 0));

	if (key[ SDL_SCANCODE_D])
		cam->accelerate(vec3(-1, 0, 0));
	 
	if (key[ SDL_SCANCODE_LSHIFT])
		cam->accelerate(vec3(0, 1, 0));

	if (key[ SDL_SCANCODE_SPACE])
		cam->accelerate(vec3(0, -1, 0));

	if (key[ SDL_SCANCODE_Q])
		cam->rotateCamera(vec3(0, 0, -16));

	if (key[ SDL_SCANCODE_E])
		cam->rotateCamera(vec3(0, 0, 16));

	 
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	handle_event: SDL input handling 
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void handle_event(const SDL_Event& event) {
	switch (event.type) { 
		case  SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				cout << "Resize" << endl;
				glViewport(0, 0, event.window.data1, event.window.data2);				
			}
			break;

		case SDL_QUIT:
			running = false;
			break;
			 
		 

		case SDL_MOUSEWHEEL:
			cam->changeSpeedLimit(event.wheel.y);
			break;
			 
	 case SDL_MOUSEMOTION: 
	 	 cam->rotateCamera(vec3(0, event.motion.xrel, 0));
	  	 cam->rotateCamera(vec3(event.motion.yrel, 0, 0));
		break;
	 case SDL_KEYDOWN:
		 handle_keyevent(event.key.keysym.sym);
	 	break;

		default:			 
			break;
	}

}



int  main(int argc, char **argv)
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	SDL setup: initialize, create window, create context
	//	
	//	A context is everything that is GL, in javascipt we had to is the gl. variable. in C++ and OpenGL the context is global
	//	
	//	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL_Init failed" << endl;
		return 	1;
	}

	 static const unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ; //|SDL_WINDOW_FULLSCREEN

	 ivec2 windowSize(1024, 768);

	SDL_Window* SDLWindow = SDL_CreateWindow(
		"Exercise 5",
		50, 50,
		windowSize.x, windowSize.y,
		flags); 

	SDL_GLContext context = SDL_GL_CreateContext(SDLWindow); 
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	
	 //cout <<"SDL_GL_GetSwapInterval "<< SDL_GL_GetSwapInterval() << endl;
	 //SDL_GL_SetSwapInterval(0);



	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //	glewInit: GLEW must always be initiallized, it defines the GL functions that we use 
	 //	
	 //	
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	GLenum err = glewInit();
	if (!GL_CHECK_ERROR("glewInit"))
		return -1;




	cout << glGetString(GL_VERSION) << endl;

	initGL();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	wireframe_mode = true;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Scene initialization: he we begin to define our scene
	//	
	//	we place a camera and a root node to which we attach all transformations
	//	
	//	we create the geometry (here, a single sphere) and attach it to scene nodes to give the geometry meaning
	//	
	//	using the transformations, we can scale, translate and rotate the geometry
	//	using material we can define the color (using textures and other shading specific information)
	//	
	//	The semantics are stored in the SceneNode: the sun is a big ! and  yellow ! sphere at the center ! of our scene  
	//	
	//	The earth is a smaller! and blue! sphere, that rotates around the center ! is positioned at a specific distance!, 
	//	has a shifted rotation axis! and rotates around that axis ! 
	//	
	//	The moon is ... it rotates around the position of the earth ! the moon revolution is unaffected by the earth rotation
	//	 That means we have to attach it at the correct node
	//	 
	//	 
	//	
	//	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 


	// set the camera:
	cam = new Camera();
	 vec3 eye = vec3(0, 180, 0);
	 vec3 focus = vec3(0, 0, 0);
	 vec3 up = vec3(0, 0, 1);


	cam->fromLookatVector(eye, focus, up);

	cam->setP(perspective(70.0f, (GLfloat)windowSize.x / (GLfloat)windowSize.y, .50f, 40000.0f));



	GSphere* sphere = new GSphere();
	sphere->create();

	GRings* rings = new GRings();
	rings->create();

	SceneNode* root = new SceneNode();
	Scene scene(root, cam);



	quat sun_tilt = angleAxis(7.25f, vec3(0,0,1));
	SceneNode* sun = new SceneNode(sphere, vec3(0), normalize(sun_tilt), 30);
	sun->material = new Material(vec4(1, 0, 0, 1));
	root->attachChild(sun);
	sun->animate = [](float deltaT, vec3& position, quat& rotation)
	{
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//  node animation: the sun does nothing but the earth shoud have an animation for revolution and rotation
		//
		//  note that we call this function using references on the rotation and position of the respective scene node
		// 
		// 
		//  check  SceneNode::updateTransformation 
		//	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	};
		 
	 
	//TODO: compose Solar System
	//check SceneNode constructor
	float earthOwnAxis = 0.7f; 
	float earthParentAxis =  0.318f;
	float moonOwnAxis = 1.f;
	float moonParentAxis = 0.1f;
	float saturnOwnAxis = 0.1f;
	float saturnParentAxis = 1.f;
	
	SceneNode* earthRevo = new SceneNode(0, vec3(0,0,70), quat() ,0);
	root->attachChild(earthRevo);
	earthRevo->animate =[](float earthParentAxis,  vec3& position, quat& transform_1){
		transform_1 = angleAxis(earthParentAxis,vec3(0,1,0));
		position = normalize(conjugate(transform_1)) * position * normalize(transform_1);
	};

	quat transform_1 = angleAxis(earthParentAxis,vec3(0,1,0));
	SceneNode* earthRot = new SceneNode(0, vec3(0), transform_1, 0);
	earthRevo->attachChild(earthRot);
	earthRot->animate =[](float earthOwnAxis,  vec3& position, quat& transform_1){
		transform_1 = transform_1 * angleAxis(earthOwnAxis,vec3(1,0,0));
		position = normalize(conjugate(transform_1)) * position * normalize(transform_1);
	};
	
	quat earth_tilt = angleAxis(23.44f, vec3(0,0,1));
	SceneNode* earth = new SceneNode(sphere,vec3(), normalize(earth_tilt),7);
	earth->material = new Material(vec4(0.5, .5, .5, 1));
	earthRot->attachChild(earth);
	earth->animate = [](float deltaT, vec3& , quat&){};
	
	
	SceneNode* moonRevo = new SceneNode(0, vec3(0,0,25),quat(), 0);
	earthRevo->attachChild(moonRevo);
	moonRevo->animate = [](float moonParentAxis, vec3& position, quat& ){
		quat transform_2 = angleAxis(moonParentAxis,vec3(0,1,0));
		position = normalize(transform_2) * position * normalize(conjugate(transform_2)); 
	};	
	
	quat transform_2 = angleAxis(moonParentAxis,vec3(0,1,0));
	SceneNode* moonRot = new SceneNode(0,vec3(0),transform_2,0);
	moonRevo->attachChild(moonRot);
	moonRot->animate = [](float moonOwnAxis, vec3& position, quat& transform_2){
		transform_2 = transform_2 * angleAxis(moonOwnAxis,vec3(1,0,0));
		position = normalize(conjugate(transform_2)) * position * normalize(transform_2);
	};
	quat moon_tilt = angleAxis(6.68f, vec3(0,1,0));
	SceneNode* moon = new SceneNode(sphere, vec3(0), normalize(moon_tilt),5);
	moon->material = new Material(vec4(0, 0, 0, 1));
	moonRot->attachChild(moon);
	moon->animate = [](float deltaT, vec3&, quat& ){};
	
	
	
	SceneNode* saturnRevo = new SceneNode(0, vec3(0,0,150),quat(), 0);
	root->attachChild(saturnRevo);
	saturnRevo->animate = [](float saturnParentAxis, vec3& position, quat& ){
		quat transform_3 = angleAxis(saturnParentAxis,vec3(0,1,0));
		position = normalize(transform_3) * position * normalize(conjugate(transform_3)); 
	};	
	
	quat transform_3 = angleAxis(saturnParentAxis,vec3(0,1,0));
	SceneNode* saturnRot = new SceneNode(0,vec3(0),transform_2,0);
	saturnRevo->attachChild(saturnRot);
	saturnRot->animate = [](float saturnOwnAxis, vec3& position, quat& transform_3){
		transform_3 = transform_3 * angleAxis(saturnOwnAxis,vec3(1,0,0));
		position = normalize(conjugate(transform_3)) * position * normalize(transform_3);
	};
	
	
	quat saturntilt = angleAxis(26.73f, vec3(0,0,1));
	SceneNode* saturn = new SceneNode(sphere, vec3(0), normalize(saturntilt),15);
	saturn->material = new Material(vec4(0.5, 0.1, 0.4, 1));
	saturnRot->attachChild(saturn);
	saturn->animate = [](float deltaT, vec3&, quat& ){};
	
	
	
	SceneNode* saturn_rings = new SceneNode(rings, vec3(0), quat(), 22);
	saturn_rings->material = new Material(vec4(0.5, 0.1, 0.4, 1));
	saturn->attachChild(saturn_rings);
	saturn_rings->animate = [](float deltaT, vec3&, quat& ){};
	
	
	
	
	
	
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// attach Camera to certain Nodes in the scene
	// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//TODO: attach camera to scenenode
	
	//root->attachCam(cam);

	 

	auto lastTime = chrono::system_clock::now();
	//cout << lastTime << endl;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RenderLoop: runs until we intereupt it using the event handler (if we forget the handler we must kill using OS)
	//
	//	SDL_GL_SwapWindow issues a sleep
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	while (running)
	{
		 
		
			SDL_Event e;
			while (SDL_PollEvent(&e)) 
			{
					handle_event(e);
			
			}
			fastUpdate();
			 
			auto now = chrono::system_clock::now();
			chrono::duration<float > deltaT = now - lastTime;
			lastTime = now;
			
			  
			  
 


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 
		// updateTransformation: every animation is done using deltaTime 
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		scene.updateTransformation(deltaT.count());


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// glClear: the  screen must be cleared every new frame
		// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// glUseProgram: we must attach a  shader
		// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUseProgram(phongShader);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// glBindBufferBase: we bind the uniform buffer to a fixed slot 0
		// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, phongShaderConstDataBuffer);
		scene.draw();

		glUseProgram(0);


		SDL_GL_SwapWindow(SDLWindow);

		
	}

	SDL_GL_DeleteContext(context);

	return true;
}

