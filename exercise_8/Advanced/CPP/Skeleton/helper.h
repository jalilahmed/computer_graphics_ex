 
 
#include <SDL_image.h>
#include <SDL2/SDL_surface.h>
 




#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "gl/glew.h" 
 
using namespace std;



bool GL_CHECK_ERROR(const char* arg)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << "GL error: " << glewGetErrorString(err) << "(" << err << ")" << std::endl;
	return err == 0;
};

class Image
{
public:


	Image(string fileName)
	{

		pixels = NULL;
		load(fileName);
	}

	~Image()
	{

		delete[]pixels;
		pixels = NULL;

	}

	unsigned int bytesPerPixel;
	unsigned int width;
	unsigned int height;
	unsigned char* pixels;

	bool load(string fileName)
	{

		SDL_Surface * image;

		image = IMG_Load(fileName.c_str());
		if (!image)
		{
			/* image failed to load */
			cout << "IMG_Load: " << IMG_GetError() << endl;
			return false;
		}


		bytesPerPixel = image->format->BytesPerPixel;
		height = image->h;
		width = image->w;


		pixels = new unsigned char[width*height*bytesPerPixel];

		memcpy(pixels, image->pixels, width*height*bytesPerPixel);


		 
		return true;
	}
};


static string textFileRead(const char *fileName)
{
	string fileString;
	string line;

	ifstream file(fileName, ios_base::in);

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);
			fileString.append(line);
			fileString.append("\n");
		}
		file.close();
	}
	else
		cout << "Unable to open " << fileName << "\n";

	return fileString;
}

// this function creates Shader Program which consists of both, a vertex and a fragment shader

int createProgram_VF(const char * name, const char *VSfile, const char *FSfile, GLuint *handle, bool forceTextShader = true)
{

	GLint compiled;
	char infoLog[4096];
	int infoLogLength;


	std::string binaryFileName(name);
	binaryFileName = binaryFileName.substr(0, binaryFileName.find("_")).append(".bin");
	std::cout << binaryFileName << std::endl;


	*handle = glCreateProgram();

	char* binary;
	GLint formats = 0;
	int len = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	GLint* binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	std::ifstream inFile(binaryFileName.c_str(), std::ofstream::binary);


	if (inFile.is_open() && !inFile.fail() && !forceTextShader)
	{
		inFile.seekg(0, inFile.end);
		len = inFile.tellg();

		inFile.seekg(0);
		std::cout << "Use Binary File: Goodbye 1+chiffre  Size: " << len << std::endl;

		binary = new char[len];

		inFile.read(binary, len);

		//for(int i=0; i< len;i++)
		//	std::cout <<binary[i];

		std::cout << std::endl;

		inFile.close();


		glProgramBinary(*handle, binaryFormats[0], binary, len);



		delete[] binary;

		//glLinkProgram(*handle); 
		GLint success;
		glGetProgramiv(*handle, GL_LINK_STATUS, &success);

		if (success)
		{
			std::cout << "Binary Loaded Successfully" << std::endl;
			return 0;
		}


		glGetProgramInfoLog(*handle, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("LinkShader: \n %s\n", infoLog);
		}

		return 0;
	}

	string codeVS = textFileRead(VSfile);
	const char *VshaderCode = codeVS.c_str();

	string codeFS = textFileRead(FSfile);
	const char *FshaderCode = codeFS.c_str();


	//compile vertex shader:
	GLuint Vshader = glCreateShader(GL_VERTEX_SHADER);



	glShaderSource(Vshader, 1, &VshaderCode, 0);
	glCompileShader(Vshader);
	glGetShaderiv(Vshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		// Print out the info log
		glGetShaderInfoLog(Vshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog for Vertex Shader %s \n%s\n", VSfile, infoLog);

		}
	}

	//compile Fragment shader:
	GLuint Fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fshader, 1, &FshaderCode, 0);
	glCompileShader(Fshader);
	glGetShaderiv(Fshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		// Print out the info log
		glGetShaderInfoLog(Fshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog for Fragment Shader %s\n%s\n", FSfile, infoLog);

		}
	}


	glAttachShader(*handle, Vshader);
	glAttachShader(*handle, Fshader);
	glDeleteShader(Vshader);
	glDeleteShader(Fshader);
	glLinkProgram(*handle);

	glGetProgramInfoLog(*handle, sizeof(infoLog), &infoLogLength, infoLog);
	if (infoLogLength > 0)
	{
		printf("LinkShader() \n%s\n", infoLog);
	}

	glProgramParameteri(*handle, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);





	glGetProgramiv(*handle, GL_PROGRAM_BINARY_LENGTH, &len);
	binary = new char[len];
	glGetProgramBinary(*handle, len, NULL, (GLenum*)binaryFormats, binary);


	std::ofstream outFile;
	outFile.open(binaryFileName.c_str(), std::ofstream::binary);
	outFile.write(binary, len);
	outFile.close();

	delete[] binary;


	std::cout << "Shader " << binaryFileName << ": " << *handle << std::endl;

	return compiled;
}

// this function creates Shader Program which consists of both, a vertex and a fragment shader

int createProgram_VGF(const char * name, const char *VSfile, const char *GSfile, const char *FSfile, GLuint *handle, bool forceTextShader = true)
{

	GLint compiled;
	char infoLog[4096];
	int infoLogLength;


	std::string binaryFileName(name);
	binaryFileName = binaryFileName.substr(0, binaryFileName.find("_")).append(".bin");
	std::cout << binaryFileName << std::endl;


	*handle = glCreateProgram();

	char* binary;
	GLint formats = 0;
	int len = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	GLint* binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	std::ifstream inFile(binaryFileName.c_str(), std::ofstream::binary);


	if (inFile.is_open() && !inFile.fail() && !forceTextShader)
	{
		inFile.seekg(0, inFile.end);
		len = inFile.tellg();

		inFile.seekg(0);
		std::cout << "Use Binary File: Goodbye 1+chiffre  Size: " << len << std::endl;

		binary = new char[len];

		inFile.read(binary, len);

		//for(int i=0; i< len;i++)
		//	std::cout <<binary[i];

		std::cout << std::endl;

		inFile.close();


		glProgramBinary(*handle, binaryFormats[0], binary, len);



		delete[] binary;

		//glLinkProgram(*handle); 
		GLint success;
		glGetProgramiv(*handle, GL_LINK_STATUS, &success);

		if (success)
		{
			std::cout << "Binary Loaded Successfully" << std::endl;
			return 0;
		}


		glGetProgramInfoLog(*handle, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("LinkShader: \n %s\n", infoLog);
		}

		return 0;
	}

	string codeVS = textFileRead(VSfile);
	const char *VshaderCode = codeVS.c_str();

	string codeGS = textFileRead(GSfile);
	const char *GshaderCode = codeGS.c_str();

	string codeFS = textFileRead(FSfile);
	const char *FshaderCode = codeFS.c_str();


	//compile vertex shader:
	GLuint Vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vshader, 1, &VshaderCode, 0);
	glCompileShader(Vshader);
	glGetShaderiv(Vshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		// Print out the info log
		glGetShaderInfoLog(Vshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog for Vertex Shader %s \n%s\n", VSfile, infoLog);

		}
	}
	//compile vertex shader:
	GLuint Gshader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(Gshader, 1, &GshaderCode, 0);
	glCompileShader(Gshader);
	glGetShaderiv(Gshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		// Print out the info log
		glGetShaderInfoLog(Gshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog for Geometry Shader %s \n%s\n", GSfile, infoLog);

		}
	}

	//compile Fragment shader:
	GLuint Fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fshader, 1, &FshaderCode, 0);
	glCompileShader(Fshader);
	glGetShaderiv(Fshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		// Print out the info log
		glGetShaderInfoLog(Fshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog for Fragment Shader %s\n%s\n", FSfile, infoLog);

		}
	}


	glAttachShader(*handle, Vshader);
	glAttachShader(*handle, Gshader);
	glAttachShader(*handle, Fshader);

	glDeleteShader(Vshader);
	glDeleteShader(Gshader);
	glDeleteShader(Fshader);
	glLinkProgram(*handle);

	glGetProgramInfoLog(*handle, sizeof(infoLog), &infoLogLength, infoLog);
	if (infoLogLength > 0)
	{
		printf("LinkShader() \n%s\n", infoLog);
	}

	glProgramParameteri(*handle, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);





	glGetProgramiv(*handle, GL_PROGRAM_BINARY_LENGTH, &len);
	binary = new char[len];
	glGetProgramBinary(*handle, len, NULL, (GLenum*)binaryFormats, binary);


	std::ofstream outFile;
	outFile.open(binaryFileName.c_str(), std::ofstream::binary);
	outFile.write(binary, len);
	outFile.close();

	delete[] binary;


	std::cout << "Shader " << binaryFileName << ": " << *handle << std::endl;

	return compiled;
}


