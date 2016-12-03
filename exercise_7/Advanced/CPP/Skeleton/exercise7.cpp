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


#include <SDL_image.h>


using namespace std;
using namespace glm;
   
//global declaration for event handling
bool wireframe_mode = false;
bool running = true;
Camera* cam; 
 

	GLuint  sunShader;

	struct SunShaderConstData
	{
		mat4 MV;
		mat4 P;
		float t;
	}  sunShaderConstData;

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
	vec3 lightSource;
	int textureCount;
} phongShaderConstData;


GLuint  earthShader;

GLuint  cloudShader;



GLuint  universeShader;

GLuint universeShaderConstDataBuffer;
struct UniverseShaderConstData
{ 
	mat4 V;
	mat4 P;
	vec3 lightSource;
	int textureCount;
} universeShaderConstData;




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

class TextureStorage
{
	public:

		~TextureStorage()
		{
			for (auto tex : textureIDs)
				glDeleteTextures(textureIDs.size(), textureIDs.data());
		}

		GLuint addTexture(string filename)
		{
			//return true;
			GLuint texID;


			Image img(filename); 

			glCreateTextures(GL_TEXTURE_2D, 1, &texID);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(texID, GL_GENERATE_MIPMAP, GL_TRUE);



			glTextureImage2DEXT(texID, GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.pixels);
			  


			textureIDs.push_back(texID);
			 

			cout << "Add Texture " << filename << ": " << texID   << endl; 
			 

			return texID;

		}
		void use()
		{ 
		  	glBindTextures(0, textureIDs.size(), textureIDs.data());
		} 

		int count()
		{
			return textureIDs.size();
		}


private:

	std::vector<GLuint> textureIDs;
};
 
 
class Material
{
public:
	Material(vec3 _kd = vec3(1, 0, 0), vec3 _ks = vec3(0, 1, 0), float _shininess = 16, float _opacity = 1)
	{
		kd = _kd;
		opacity = _opacity;
		ks = _ks;
		shininess = _shininess;
	}

	vec3	kd;
	float	opacity;
	vec3	ks;
	float	shininess;
	TextureStorage textures;
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

				phongShaderConstData.textureCount = material->textures.count();

				glNamedBufferSubData(phongShaderConstDataBuffer, 0, sizeof(PhongShaderConstData), &phongShaderConstData);


				glUseProgram(shader);

				if (shader == universeShader)
				{ 
					glNamedBufferSubData(universeShaderConstDataBuffer, 0, sizeof(UniverseShaderConstData), &universeShaderConstData);
					glBindBufferBase(GL_UNIFORM_BUFFER, 3, universeShaderConstDataBuffer);

				}
				else if (shader == sunShader)
				{

					sunShaderConstData.MV = sunShaderConstData.MV * glm::scale(scale, scale, scale);
					glNamedBufferSubData(sunShaderConstDataBuffer, 0, sizeof(SunShaderConstData), &sunShaderConstData);

					glBindBufferBase(GL_UNIFORM_BUFFER, 2, sunShaderConstDataBuffer);


				}
				glBindBufferBase(GL_UNIFORM_BUFFER, 0, phongShaderConstDataBuffer);
				
				material->textures.use();
					  

				std::vector<int> textureIndices(material->textures.count());
				auto uniformLocation = glGetUniformLocation(shader, "textures");

				for (int t = 0; t< material->textures.count(); t++)
				{
					textureIndices[t] = t;
				}
				glUniform1iv(uniformLocation, textureIndices.size(), textureIndices.data());

					

				GL_CHECK_ERROR("Draw");

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
			 	phongShaderConstData.kd = (material != 0) ? material->kd : vec3(1,  0,0 );

				phongShaderConstData.textureCount = material->textures.count();

			 	glNamedBufferSubData(phongShaderConstDataBuffer, 0, sizeof(phongShaderConstData), &phongShaderConstData);

			  	glUseProgram(shader);
			  	glBindBufferBase(GL_UNIFORM_BUFFER, 0, phongShaderConstDataBuffer);
			  	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, phongShaderInstancedTransformationDataBuffer);
				material->textures.use();


				std::vector<int> textureIndices(material->textures.count());
				auto uniformLocation = glGetUniformLocation(shader, "textures");

				for (int t = 0; t< material->textures.count(); t++)
				{
					textureIndices[t] = t;
				}
				glUniform1iv(uniformLocation, textureIndices.size(), textureIndices.data());


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
	glClearColor(3.f/255, 3.f / 255, 3.f / 255, 1);   // set the clear color to black
	glEnable(GL_DEPTH_TEST); // turn on the depth test
	glEnable(GL_CULL_FACE);  // turn on backface culling

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0, -.2); // WHAT IS A PROPER NUMBER===
	 

	// create the shaders (the functions are defined in helper.h)





	 createProgram_VF("phong","shader/phong_VS.glsl", "shader/phong_FS.glsl", &phongShader, true);

	 createProgram_VF("asteroid","shader/phongInstanced_VS.glsl", "shader/phong_FS.glsl", &phongShaderInstanced, true);
	 createProgram_VF("earthTextured", "shader/earthTextured_VS.glsl", "shader/earthTextured_FS.glsl", &earthShader, true);
	 createProgram_VF("clouds", "shader/phong_VS.glsl", "shader/clouds_FS.glsl", &cloudShader, true);

	 glCreateBuffers(1, &phongShaderConstDataBuffer);
	 glNamedBufferData(phongShaderConstDataBuffer, sizeof(phongShaderConstData), &phongShaderConstData, GL_STATIC_DRAW);
  


	createProgram_VF("sun", "shader/sun_VS.glsl", "shader/sun_FS.glsl", &sunShader, true);
	glCreateBuffers(1, &sunShaderConstDataBuffer);
	glNamedBufferData(sunShaderConstDataBuffer, sizeof(SunShaderConstData), &sunShaderConstData, GL_STATIC_DRAW);
 


	createProgram_VF("universe", "shader/universe_VS.glsl", "shader/universe_FS.glsl", &universeShader, true);

	glCreateBuffers(1, &universeShaderConstDataBuffer);
	glNamedBufferData(universeShaderConstDataBuffer, sizeof(UniverseShaderConstData), &universeShaderConstData, GL_STATIC_DRAW);
	//

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

	static const unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE  | SDL_WINDOW_FULLSCREEN;

	  ivec2 windowSize(3440, 1440);
	// ivec2 windowSize(1920, 1080);

	SDL_Window* SDLWindow = SDL_CreateWindow(
		"Exercise 5",
		50, 50,
		windowSize.x, windowSize.y,
		flags); 

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
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
	 vec3 eye = vec3(-5, 0, 69);
	 vec3 focus = vec3(0, 0, 70);
	 vec3 up = vec3(0, 1, 0);


	cam->fromLookatVector(eye, focus, up);

	cam->setP(perspective(70.0f, (GLfloat)windowSize.x / (GLfloat)windowSize.y, .05f, 4000.0f));
	 


	GSphere* sphere = new GSphere();
	sphere->createConfigurable();


	GSphere* sphereHP = new GSphere();
	sphereHP->createConfigurable(2048, 1024);

	GQuad* quad = new GQuad();
	quad->create();

	SceneNode* root = new SceneNode();
	Scene scene(root, cam);


	SceneNode* universe = new SceneNode(quad);
	universe->material = new Material(vec3(1));
	universe->shader = universeShader;
	universe->material->textures.addTexture("../textures/universe.jpg");
	
	root->attachChild(universe);

	SceneNode* sun = new SceneNode(sphere, vec3(0), quat(), 5);
	sun->material = new Material(vec3(.74, .7, 0));
	sun->shader = sunShader;
	root->attachChild(sun);
	 

	SceneNode* earthRev = new SceneNode(0,vec3(0,0, 70 ));
	earthRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = .0f / M_PI * deltaT;

		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));

		position = glm::rotate(revolution, position);// revolution * position *  conjugate(revolution);
	};

	root->attachChild(earthRev);



	 SceneNode* earthTilt = new SceneNode();
	 earthTilt->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	 {
	  
	 	rotation = normalize(angleAxis(22.f / 180.f *(float)(M_PI ), vec3(-1, 0, 0)));
	 };
	 earthRev->attachChild(earthTilt);

	SceneNode* earthRot = new SceneNode();
	earthRot->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = .50f / M_PI * deltaT;
		 
		quat r = normalize(angleAxis(angle, vec3(0, 1, 0)));

		rotation = normalize(r * rotation);
	};
	earthTilt->attachChild(earthRot);

	SceneNode* earth = new SceneNode(sphereHP, vec3(0), quat(), 2);
	earth->material = new Material(vec3(.5, .5, .9));
	earth->material->textures.addTexture("../textures/earth.jpg");
	earth->material->textures.addTexture("../textures/earthNight.jpg");
	earth->material->textures.addTexture("../textures/earthClouds.jpg");
	earth->material->textures.addTexture("../textures/earthSpec.jpg");
	earth->material->textures.addTexture("../textures/earthBump.jpg"); 

	earth->shader = earthShader;
	earthRot->attachChild(earth);


	SceneNode* clouds = new SceneNode(sphere, vec3(0), quat(), 2.3);
	clouds->material = new Material(vec3(1));
	clouds->material->textures.addTexture("../textures/earthClouds.jpg");

	clouds->shader = cloudShader;
	earthRot->attachChild(clouds);

	
	SceneNode* moonRev = new SceneNode(0, vec3(0, 0, -10));
	moonRev->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	{
		float angle = 1.0f / M_PI * deltaT;
	
		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));
	
		 position =  revolution * position  * conjugate(revolution);
	};
	earthRot->attachChild(moonRev);

	SceneNode* moon = new SceneNode(sphere, vec3(0), quat(), .7);
	moon->material = new Material(vec3(.5, .5, .5));
	moon->material->textures.addTexture("../textures/moon.jpg");

	moon->shader = phongShader;
	 moonRev->attachChild(moon);

	 



	GMesh* asteroidMesh = new GMesh();
	asteroidMesh->create("Asteroid.obj");


 	InstancedSceneNode* asteroidBelt = new InstancedSceneNode(asteroidMesh, vec3(0, 0, 0 ));
	asteroidBelt->material = new Material(vec3(.7,.7,.7));
	asteroidBelt->material->textures.addTexture("../textures/asteroid.jpg") ;
	asteroidBelt->shader = phongShaderInstanced;
	
	asteroidBelt->animate = [](float deltaT, glm::vec3& position, glm::quat& rotation)
	 {
 		float angle = .015f / M_PI * deltaT;
 
 		quat revolution = normalize(angleAxis(angle, vec3(0, 1, 0)));
 
		rotation = normalize(revolution * rotation);
	 };

	int slices = 8000;
	float dR = 2*M_PI / slices;
	for (int i = 0; i < slices; i++)
	{
		auto rz = normalize( angleAxis(float(float(rand()) / RAND_MAX * 2.f * M_PI), vec3(0, 0, 1))  );

		vec3 pjitter = rotate(rz, vec3( 2 , 0, 0));  
		vec3 pDistance = vec3(250 +  .001* ( ( 1 - float(rand()*rand()))) / RAND_MAX, 0, 0);

		Transform asteroid;
		asteroid.position = rotate(normalize(angleAxis(i*dR, vec3(0, 1, 0))), pDistance + pjitter);


		asteroid.rotation = normalize( quat(.5f - float((rand()) / RAND_MAX), 
			.5f - float(rand()) / RAND_MAX,
			.5f - float(rand()) / RAND_MAX,
			.5f - float(rand()) / RAND_MAX));
		 



		asteroid.scale =  pow(float(rand()) / RAND_MAX, 2);
		asteroidBelt->addTransform(asteroid);

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
		phongShaderConstData.lightSource = vec3(0, 0, 0 );
		


		universeShaderConstData.V = cam->V();
		universeShaderConstData.P = cam->P(); 
		universeShaderConstData.lightSource = vec3(0, 0, 0);

		sunShaderConstData.P = cam->P();
		sunShaderConstData.MV = cam->V();
		sunShaderConstData.t += deltaT.count();


		scene.draw();


		 




		SDL_GL_SwapWindow(SDLWindow);

		
	}

	SDL_GL_DeleteContext(context);

	return true;
}

