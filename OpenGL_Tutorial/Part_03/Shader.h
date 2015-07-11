#pragma once
 
#include <GL/glew.h>
#include <SDL2/SDL.h>
 
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
 
class Shader
{
public:
 
	void BindAttributeLocation(int index, const std::string &attribute)
	{
		// Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
		// Attribute locations must be setup before calling glLinkProgram
		glBindAttribLocation(shaderProgram, index, attribute.c_str());
	}

	uint32_t GetAttributeId(const std::string &attributeName)
	{
		return glGetAttribLocation(shaderProgram, attributeName.c_str() );
	}
 
	void UseProgram()
	{
		// Load the shader into the rendering pipeline
		glUseProgram(shaderProgram);
	}

	bool Init()
	{
		// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
		shaderProgram = glCreateProgram();

		if (!LoadShader("tutorial2.vert", GL_VERTEX_SHADER))
			return false;

		if (!LoadShader("geom.geom", GL_GEOMETRY_SHADER)) return false;

		if (!LoadShader("tutorial2.frag", GL_FRAGMENT_SHADER))
			return false;

		// All shaders has been create, now we must put them together into one large object
		return LinkShaders();
	}

 	bool LoadShader(const std::string &fileName, GLenum shaderType)
	{
		std::cout << "Linking Shader : " << fileName << std::endl;

		int shaderId = CreateShader(fileName, shaderType);
 
		if (TryCompileShader(shaderId))
		{
			glAttachShader(shaderProgram, shaderId);
			shaderIds.push_back(shaderId);
			return true;
		}

		return false;
	}

	void CleanUp()
	{
		/* Cleanup all the things we bound and allocated */
		glUseProgram(0);

		for ( auto i : shaderIds)
			glDetachShader(shaderProgram, i);

		glDeleteProgram(shaderProgram);

		for ( auto i : shaderIds)
			glDeleteShader(i);
	}

private:
	bool TryCompileShader(int shaderId)
	{
		// Compile the vertex shader
		glCompileShader(shaderId);
 
		int wasCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &wasCompiled);
 

		PrintShaderCompilationErrorInfo(shaderId);
		if (wasCompiled == 0)
		{
			return false;
		}

		return true;
	}
	int CreateShader(const std::string &fileName, GLenum shaderType)
	{
		// Read file as std::string 
		std::string str = ReadFile(fileName.c_str());
 
		 // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		int32_t size = str.length();
		
		// Create an empty vertex shader handle
		int shaderId = glCreateShader(shaderType);

		// Send the vertex shader source code to OpenGL
		glShaderSource(shaderId , 1, &src, &size);

		return shaderId;
	}

	std::string ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);
 
		// Read file into buffer
		std::stringstream buffer;
		buffer << t.rdbuf();
 
		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();
 
		return fileContent;
	}

	bool LinkShaders()
	{
		// Link. At this point, our shaders will be inspected/optized and the binary code generated
		// The binary code will then be uploaded to the GPU
		glLinkProgram(shaderProgram);
 
		// Verify that the linking succeeded
		int isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

		if (isLinked == false)
			PrintShaderLinkingError(shaderProgram);

		return isLinked != 0;
	}

	void PrintShaderLinkingError(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader linking failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::cout << "Info Length : " << maxLength << std::endl;

		// Get shader info log
		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;
		return;
	}

	// If something went wrong whil compiling the shaders, we'll use this function to find the error
	void PrintShaderCompilationErrorInfo(int32_t shaderId)
	{

		// Find length of shader info log
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// Get shader info log
		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

		std::string log = shaderInfoLog;
 
		if (log.length())
		{
			std::cout << "=======================================\n";
			std::cout <<  shaderInfoLog << std::endl;
			std::cout << "=======================================\n\n";
		}
		// Print shader info log
		delete shaderInfoLog;
	}
 
	// The handle to our shader program
	GLuint shaderProgram;
 
	std::vector<int32_t> shaderIds;
	// The handles to the induvidual shader
	GLuint vertexshader, fragmentShader;
 
};
