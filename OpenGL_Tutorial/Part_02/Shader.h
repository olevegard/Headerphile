#pragma once
 
#include <GL/glew.h>
#include <SDL2/SDL.h>
 
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
 
class Shader
{
public:
	std::string ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);
 
		// Read file into buffer
		std::stringstream buffer;
		buffer << t.rdbuf();
 
		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();
		fileContent += '\0';
 
		return fileContent;
	}
 
	void BindAttributeLocation(int index, const std::string &attribute)
	{
		// Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
		// Attribute locations must be setup before calling glLinkProgram
		glBindAttribLocation(shaderprogram, index, attribute.c_str());
	}
 
	void Init()
	{
		LoadVertexShader("tutorial2.vert");
		LoadFragmentShader("tutorial2.frag");
 
		/* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
		/* We must link them together to make a GL shader program */
		/* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
		/* Assign our program handle a "name" */
		shaderprogram = glCreateProgram();
 
		/* Attach our shaders to our program */
		glAttachShader(shaderprogram, vertexshader);
		glAttachShader(shaderprogram, fragmentShader);
 
		LinkShaders();
	}
 
	void UseProgram()
	{
		/* Load the shader into the rendering pipeline */
		glUseProgram(shaderprogram);
	}
 
	void LoadVertexShader(const std::string &filename)
	{
		std::cout << "Linking vert shader" << std::endl;
		// Read file as 
		std::string str = ReadFile(filename.c_str());
 
		 // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		
		/* Create an empty vertex shader handle */
		vertexshader = glCreateShader(GL_VERTEX_SHADER);
 
		/* Send the vertex shader source code to GL */
		/* Note that the source code is NULL character terminated. */
		/* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
		glShaderSource(vertexshader, 1, &src, 0);
 
		/* Compile the vertex shader */
		glCompileShader(vertexshader);
 
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
 
		if (IsCompiled_VS == false)
		{
			std::cout << "Vertex Shader compilation failed : " << vertexshader << std::endl;
 
			glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
 
			/* The maxLength includes the NULL character */
			vertexInfoLog = (char *)malloc(maxLength);
 
			glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
 
			std::cout << "info : " << vertexInfoLog << std::endl;
 
			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			free(vertexInfoLog);
			return;
		}
 
	}

	void LoadFragmentShader(const std::string &filename)
	{
		std::cout << "Linking frag shader" << std::endl;
		// Read file as 
		std::string str = ReadFile(filename.c_str());
 
		 // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		
		/* Create an empty vertex shader handle */
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
		/* Send the vertex shader source code to GL */
		/* Note that the source code is NULL character terminated. */
		/* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
		glShaderSource(fragmentShader, 1, &src, 0);
 
		/* Compile the vertex shader */
		glCompileShader(fragmentShader);
 
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &IsCompiled_FS);
 
		if (IsCompiled_FS == false)
		{
			std::cout << "Fragment Shader compilation failed : " << fragmentShader << std::endl;
 
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
 
			/* The maxLength includes the NULL character */
			vertexInfoLog = (char *)malloc(maxLength);
 
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, vertexInfoLog);
 
			std::cout << "info : " << vertexInfoLog << std::endl;
 
			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			free(vertexInfoLog);
			return;
		}
 
	}
	void LinkShaders()
	{
		/* Link our program */
		/* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
		/* The binary code is uploaded to the GPU, if there is no error. */
		glLinkProgram(shaderprogram);
 
		/* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
		/* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
		/* too many texel fetch instructions or too many interpolators or dynamic loops. */
 
		glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
		if (IsLinked == false)
		{
			std::cout << "Not linked : shaderprogram" << std::endl;
 
			/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
			glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
 
			/* The maxLength includes the NULL character */
			shaderProgramInfoLog = (char *)malloc(maxLength);
 
			/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
			glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);

			std::cout << "MSG : " << shaderProgramInfoLog << std::endl;
 
			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			free(shaderProgramInfoLog);
			return;
		}
	}
 
	void CleanUp()
	{
		/* Cleanup all the things we bound and allocated */
		glUseProgram(0);
		glDetachShader(shaderprogram, vertexshader);
		glDetachShader(shaderprogram, fragmentShader);

		glDeleteProgram(shaderprogram);

		glDeleteShader(vertexshader);
		glDeleteShader(fragmentShader);

		free(vertexsource);
	}
 
	// The handle to our shader program
	GLuint shaderprogram;
 
	// The handles to the induvidual shader
	GLuint vertexshader, fragmentShader;
 
	// The source code of the shaders
	GLchar *vertexsource, *fragmentSource;
 
	int IsCompiled_VS, IsCompiled_FS;
 
	int maxLength;
	char *vertexInfoLog;
	char *shaderProgramInfoLog;
 
	int IsLinked;
};
