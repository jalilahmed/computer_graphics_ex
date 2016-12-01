


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


//assimp stuff


#include <assimp/assimp/Importer.hpp>      // C++ importer interface
#include <assimp/assimp/scene.h>           // Output data structure
#include <assimp/assimp/postprocess.h>     // Post processing flags


using namespace std;
using namespace glm;
 

//global declaration for event handling
bool wireframe_mode = false;
bool running = true;
Camera* cam; 
 

	GLuint  sunShader;

	struct SunShaderConstData
	{
		mat4 MVP;
		float t;
	} sunShaderConstData;
	GLuint sunShaderConstDataBuffer;



GLuint  phongShader;  

GLuint phongShaderConstDataBuffer;
struct PhongShaderConstData
{
	mat4 M;
	mat4 V;
	mat4 P;

	vec3 kd;
	float opacity;
	vec3 ks;	
	float shininess;
	vec4 lightSource;
} phongShaderConstData;


GLuint  phongShaderInstanced;
GLuint phongShaderInstancedTransformationDataBuffer;

// BEWARE THE FLOAT4 ALIGNMENT, THIS IS THE SINGLE MOST  IMPORTANT INFORMATION TO TAKE AWAY FROM THIS, note that float before vec3 will not work
struct Transform
{
	Transform()
	{
		scale  = 1;
	}; 
	quat rotation;
	vec3 position;
	float scale;
};


GLuint  ringsShader;
 
 

class Material
{
public:
	Material(vec3 _kd = vec3(1, 0, 0), vec3 _ks = vec3(0, 1, 0), float _shininess = 16, float _opacity = 1)
	{
		kd= _kd;
		opacity = _opacity;
		ks = _ks;
		shininess = _shininess;
	}

	vec3	kd;
	float	opacity;
	vec3	ks;
	float	shininess;
};


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

				auto q = quat_cast(mat3(M));
			 
				auto rotation = angleAxis(-angle(q), axis(q));
				auto position = -vec3(M * vec4(vec3(0), 1.f))   ;

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
		 

		virtual void draw()
		{

			if (shader != 0)
			{ 

				phongShaderConstData.M = M * glm::scale(scale, scale, scale);;
				phongShaderConstData.kd = (material != 0) ? material->kd : vec3(1, 0, 0);
				glNamedBufferSubData(phongShaderConstDataBuffer, 0, sizeof(PhongShaderConstData), &phongShaderConstData);

				glUseProgram(shader);
				 
				if (shader == sunShader)
				{

					sunShaderConstData.MVP = sunShaderConstData.MVP * glm::scale(scale, scale, scale);
					glNamedBufferSubData(sunShaderConstDataBuffer, 0, sizeof(SunShaderConstData), &sunShaderConstData);

					glBindBufferBase(GL_UNIFORM_BUFFER, 2, sunShaderConstDataBuffer);

				}
				else
				{
					glBindBufferBase(GL_UNIFORM_BUFFER, 0, phongShaderConstDataBuffer);

				}

			}
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
		 GLuint shader = 0;
	protected:

		float scale;
		quat rotation;
		vec3 position;


		mat4 M;
		vector<SceneNode*> children;
		Geometry* geometry = 0;
		Camera*  cam = 0;


};


class InstancedSceneNode : public  SceneNode
{
	public:
		InstancedSceneNode(Geometry* _geometry = 0, const vec3& _position = vec3(0), const quat& _rotation = quat(), float _scale = 1):
			SceneNode(_geometry, _position, _rotation, _scale)
		{
			glCreateBuffers(1, &phongShaderInstancedTransformationDataBuffer);
			

			
		}
		~InstancedSceneNode()
		{
			glDeleteBuffers(1, &phongShaderInstancedTransformationDataBuffer);
		}

		int instanceCount()
		{
			return instanceTransformations.size();
		}

		void addTransform(const Transform& transform)
		{
			instanceTransformations.push_back(transform);
		}

		void upLoadInstanceData()
		{

			glNamedBufferData(phongShaderInstancedTransformationDataBuffer, sizeof(Transform) * instanceTransformations.size(), instanceTransformations.data(), GL_STATIC_DRAW);
			
			 
		}


		void draw()
		{  
			if(shader != 0)
			{
			 	phongShaderConstData.M = M * glm::scale(scale, scale, scale);;
				phongShaderConstData.kd = (material != 0) ? material->kd : vec3(1, 0, 0);
				phongShaderConstData.ks = (material != 0) ? material->ks : vec3(1, 0, 0);
				phongShaderConstData.opacity = (material != 0) ? material->opacity : 1;
				phongShaderConstData.shininess = (material != 0) ? material->shininess : 1;
			 	glNamedBufferSubData(phongShaderConstDataBuffer, 0, sizeof(phongShaderConstData), &phongShaderConstData);
			  
			  	glUseProgram(shader);
			  	glBindBufferBase(GL_UNIFORM_BUFFER, 0, phongShaderConstDataBuffer);
			  	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, phongShaderInstancedTransformationDataBuffer);
			}



			if (geometry != 0)
			{ 
				 geometry->draw(instanceCount());
			} 
			for (auto child : children)
			{
				child->draw();
			}
		}



	protected:

		std::vector<Transform> instanceTransformations;


};

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

			root->draw();
		}

		SceneNode* root;
		Camera * cam;


};


  
 
void initGL() {
	glClearDepth(1);
	glClearColor(.95, .95, .95, .95);   // set the clear color to black
	glEnable(GL_DEPTH_TEST); // turn on the depth test
	glEnable(GL_CULL_FACE);  // turn on backface culling



	 

	// create the shaders (the functions are defined in helper.h)




	  createProgram_VF("sun","shader/sun_VS.glsl", "shader/sun_FS.glsl", &sunShader, true);


	 createProgram_VF("phong","shader/phong_VS.glsl", "shader/phong_FS.glsl", &phongShader, true);
	 createProgram_VF("asteroid","shader/phongInstanced_VS.glsl", "shader/phong_FS.glsl", &phongShaderInstanced, true);
 
	 glCreateBuffers(1, &phongShaderConstDataBuffer);
	 glNamedBufferData(phongShaderConstDataBuffer, sizeof(phongShaderConstData), &phongShaderConstData, GL_STATIC_DRAW);
 
 
	 

	glCreateBuffers(1, &sunShaderConstDataBuffer);
	glNamedBufferData(sunShaderConstDataBuffer, sizeof(SunShaderConstData), &sunShaderConstData, GL_STATIC_DRAW);
	 //

	
	createProgram_VF("rings", "shader/phong_VS.glsl", "shader/phongRings_FS.glsl", &ringsShader, true);

}

 
void handle_keyevent(const SDL_Keycode& keycode) {

	switch (keycode) { 

	case SDLK_ESCAPE: 
		running = false;
		break;

	case SDLK_p:

		wireframe_mode = !wireframe_mode;
		if (wireframe_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		cout << "Switch Wireframe Mode: " << (wireframe_mode ? "on" : "off") << endl;

		break;


	default:
		break;
	}
	 
	 
	 


}

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

		default:
				;
				//break;
	}

	 


}



int  main(int argc, char **argv)
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL_Init failed" << endl;
		return 	1;
	}

	 static const unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ;

	 ivec2 windowSize(1024, 768);

	SDL_Window* SDLWindow = SDL_CreateWindow(
		"Exercise 5",
		50, 50,
		windowSize.x, windowSize.y,
		flags); 

	SDL_GLContext context = SDL_GL_CreateContext(SDLWindow); 
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	 cout <<"SDL_GL_GetSwapInterval "<< SDL_GL_GetSwapInterval() << endl;
	 //SDL_GL_SetSwapInterval(0);

	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (!GL_CHECK_ERROR("glewInit"))
		return -1;


	cout << glGetString(GL_VERSION) << endl;

	initGL();
	 


	// init Scene


	// set the camera:
	cam = new Camera();
	 vec3 eye = vec3(0, 0, 180);
	 vec3 focus = vec3(0, 0, 0);
	 vec3 up = vec3(0, 1, 0);


	cam->fromLookatVector(eye, focus, up);

	cam->setP(perspective(70.0f, (GLfloat)windowSize.x / (GLfloat)windowSize.y, .50f, 40000.0f));
	 


	GSphere* sphere = new GSphere();
	sphere->create();

	SceneNode* root = new SceneNode();
	Scene scene(root, cam);

	SceneNode* sun = new SceneNode(sphere, vec3(0), quat(), 6);
	sun->material = new Material(vec3(.74, .7, 0));
	sun->shader = sunShader;
	root->attachChild(sun);
	 
 


	//Merkur - Mercury
	//~116 Tage umlaufzeit
	//58Tage pro Rotation
	SceneNode* merkurRev = new SceneNode(0, vec3(0, 0, 30));
	merkurRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 3.14 / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(merkurRev);


	SceneNode* merkurRot = new SceneNode();
	merkurRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = (1.0 / 58.0) / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 1, 0)));

		rotation = normalize(r * rotation);
	};
	merkurRev->attachChild(merkurRot);

	SceneNode* merkur = new SceneNode(sphere, vec3(0), quat(), 2);
	merkur->material = new Material(vec3(.5, .5, .5));
	merkur->shader = phongShader;
	merkurRot->attachChild(merkur);

	//Venus 583 Tage -> 0.21f
	//1 umdrehung / halbe stunde -> 48 * umdrehungen der Erde
	//Achse um 177° geneigt
	SceneNode* venusRev = new SceneNode(0, vec3(0, 0, 70));
	venusRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.63f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(venusRev);


	SceneNode* venusRot = new SceneNode();
	venusRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 48.0f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, -0.99863, 0.052336)));

		rotation = normalize(r * rotation);
	};
	venusRev->attachChild(venusRot);

	SceneNode* venus = new SceneNode(sphere, vec3(0), quat(), 4);
	venus->material = new Material(vec3(.75, .72, .70));
	venus->shader = phongShader;
	venusRot->attachChild(venus);


	//Erde 365 Tage
	//Achse um 28° geneigt
	SceneNode* earthRev = new SceneNode(0, vec3(0, 0, 100));
	earthRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.0 / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(earthRev);


	SceneNode* earthRot = new SceneNode();
	earthRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.0f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.882948f, 0.469472f)));

		rotation = normalize(r * rotation);
	};
	earthRev->attachChild(earthRot);

	SceneNode* earth = new SceneNode(sphere, vec3(0), quat(), 8);
	earth->material = new Material(vec3(.5, .5, .9));
	earth->shader = phongShader;
	earthRot->attachChild(earth);

	//Mond
	SceneNode* moonRev = new SceneNode(0, vec3(0, 0, -10.5));
	moonRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 3.6f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = revolution * position  * conjugate(revolution);
	};

	earthRev->attachChild(moonRev);

	SceneNode* moonRot = new SceneNode();
	moonRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 3.6f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.992546, 0.121869)));

		rotation = normalize(r * rotation);
	};
	moonRev->attachChild(moonRot);

	SceneNode* moon = new SceneNode(sphere, vec3(0), quat(), 2);
	moon->material = new Material(vec3(.5, .5, .5));
	moon->shader = phongShader;
	moonRot->attachChild(moon);

	
	//Mars 780 Tage -> 0.21f
	//24h
	//Achse um 25° geneigt
	SceneNode* marsRev = new SceneNode(0, vec3(0, 0, 130));
	marsRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.46f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(marsRev);


	SceneNode* marsRot = new SceneNode();
	marsRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.0f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.906308, 0.422618)));

		rotation = normalize(r * rotation);
	};
	marsRev->attachChild(marsRot);

	SceneNode* mars = new SceneNode(sphere, vec3(0), quat(), 5);
	mars->material = new Material(vec3(.9, .2, .2));
	mars->shader = phongShader;
	marsRot->attachChild(mars);

	//Jupiter 399 Tage
	//10h
	//Achse um 3° geneigt
	SceneNode* jupiterRev = new SceneNode(0, vec3(0, 0, 500));
	jupiterRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.91f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(jupiterRev);


	SceneNode* jupiterRot = new SceneNode();
	jupiterRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 2.4f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.99863, 0.052336)));

		rotation = normalize(r * rotation);
	};
	jupiterRev->attachChild(jupiterRot);

	SceneNode* jupiter = new SceneNode(sphere, vec3(0), quat(), 30.0);
	jupiter->material = new Material(vec3(.75, .70, .69));
	jupiter->shader = phongShader;
	jupiterRot->attachChild(jupiter);

	//Saturn 378 Tage
	//10h rotation
	//Achse um 27° geneigt
	SceneNode* saturnRev = new SceneNode(0, vec3(0, 0, 900));
	saturnRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.965f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(saturnRev);

	SceneNode* saturnRot = new SceneNode();
	saturnRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 2.2f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.882948, 0.469472)));

		rotation = normalize(r * rotation);
	};
	saturnRev->attachChild(saturnRot);

	SceneNode* saturn = new SceneNode(sphere, vec3(0), quat(), 9.0);
	saturn->material = new Material(vec3(.89, .88, .75));
	saturn->shader = phongShader;
	saturnRot->attachChild(saturn);

	{
		GRings* rings = new GRings();
		rings->create();

		SceneNode* saturnRings = new SceneNode(rings, vec3(0), quat(), 12.0);
		saturnRings->material = new Material(vec3(.80, .72, .72));
		saturnRings->shader = ringsShader;
		saturn->attachChild(saturnRings);
	}

	{
		GRings* rings = new GRings();
		rings->create();

		SceneNode* saturnRings = new SceneNode(rings, vec3(0), quat(), 14.0);
		saturnRings->material = new Material(vec3(.75, .72, .70));
		saturnRings->shader = ringsShader;

		saturn->attachChild(saturnRings);
	}



	//Uranus 369 Tage
	//17h
	//Achse um 3° geneigt
	SceneNode* uranusRev = new SceneNode(0, vec3(0, 0, 1800));
	uranusRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.989f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(uranusRev);


	SceneNode* uranusRot = new SceneNode();
	uranusRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.4f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, -0.121869, 0.992546)));

		rotation = normalize(r * rotation);
	};
	uranusRev->attachChild(uranusRot);

	SceneNode* uranus = new SceneNode(sphere, vec3(0), quat(), 31);
	uranus->material = new Material(vec3(.2, .2, .70));
	uranus->shader = phongShader;
	uranusRot->attachChild(uranus);

	//Neptun 367 Tage
	//16h
	//Achse um 28,32° geneigt
	SceneNode* neptunRev = new SceneNode(0, vec3(0, 0, 3000));
	neptunRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 0.995f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(neptunRev);


	SceneNode* neptunRot = new SceneNode();
	neptunRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.5f / M_PI * deltaT;

		quat r = normalize(angleAxis(angle, vec3(0, 0.882948, 0.469472)));

		rotation = normalize(r * rotation);
	};
	neptunRev->attachChild(neptunRot);

	SceneNode* neptun = new SceneNode(sphere, vec3(0), quat(), 25.0);
	neptun->material = new Material(vec3(.0, .0, 1.));
	neptun->shader = phongShader;
	neptunRot->attachChild(neptun);


	GMesh* asteroidMesh = new GMesh();
	asteroidMesh->create("Asteroid.obj");

	
 	InstancedSceneNode* asteroidBelt = new InstancedSceneNode(asteroidMesh, vec3(0) , quat(), 1);
	asteroidBelt->material = new Material(vec3(.7,0,0));
	asteroidBelt->shader = phongShaderInstanced;

	asteroidBelt->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	 {
 		float angle = .025f / M_PI * deltaT;
 
 		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));
 
		rotation = normalize(revolution * rotation);
	 };

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO:  distribute asteroids
	//
	// remove the scaling factor (set back to 1), we vary the scale using the Transform struct
	// populate transformation vector like this (check class)
	// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	Transform asteroid;	
	for(int i = 0  ; i < 8000 ; i++)
	{
	float y = float(rand() % (5 + 1 - 1) + 1);
	float z = float(rand() % (450 + 1 - 350) + 350); 
	if(length(normalize(vec3(0,y,z))) < 1.f)
	{
	quat uniRot = angleAxis(float((i*2*3.1415)/8000),vec3(0,1,0)); 
	asteroid.position = rotate(uniRot,vec3(0,y,z));
	asteroid.rotation = angleAxis(float(rand())/RAND_MAX,vec3(0,1,0));
	asteroid.scale = float(rand() % (3 + 1 - 1) + 1) ;
	asteroidBelt->addTransform(asteroid);
	}
	}
 	asteroidBelt->upLoadInstanceData();
  	root->attachChild(asteroidBelt);
	

	auto lastTime = chrono::system_clock::now();
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
			
			  
			  
 


		scene.updateTransformation(deltaT.count());
		 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		phongShaderConstData.V = cam->V();
		phongShaderConstData.P = cam->P();
		phongShaderConstData.lightSource = vec4(0, 0, 0, 1);
		
		
		scene.draw();


		 


		sunShaderConstData.MVP = cam->P()* cam->V();
		sunShaderConstData.t += deltaT.count();


		SDL_GL_SwapWindow(SDLWindow);

		
	}

	SDL_GL_DeleteContext(context);

	return true;
}

