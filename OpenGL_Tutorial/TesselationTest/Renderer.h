#pragma once

#include <iostream>
#include <string>

#include "Shader.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL2/SDL.h>

#include "../EventHandler.h"

#include "Model.h"
 

std::string programName = "Headerphile SDL2 - OpenGL thing";

class Renderer
{
	public :
	Renderer()
	{
		projection = glm::perspective(45.0f, 4.0f / 4.0f, 0.1f, 100.0f);
		view =
			glm::lookAt
			(
			 glm::vec3(0,0,-9), // Camera is at (4,3,3), in World Space
			 glm::vec3(0,0,0), // and looks at the origin
			 glm::vec3(0,-1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
	}
	bool Init()
	{
		// Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to init SDL\n";
			return false;
		}

		// Create our window centered at 512x512 resolution
		mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Check that everything worked out okay
		if (!mainWindow )
		{
			std::cout << "Unable to create window\n"<< std::endl;;
			CheckSDLError(__LINE__);
			return false;
		}

		SetSDL_GL_Attributes();

		// Create our opengl context and attach it to our window
		mainContext = SDL_GL_CreateContext(mainWindow );

		// This makes our buffer swap syncronized with the monitor's vertical refresh
		// 		( which means it enables v-sync)
		// Setting this to 0 will disable V-sync
		// 		Which means our application could run at unlimited fps
		SDL_GL_SetSwapInterval(1);

		// Init GLEW
		glewExperimental = GL_TRUE; 
		glewInit();

		std::cout << "Seting up VBO + VAO..." << std::endl;

		SetOpenGLOptions();

		if (!CreateSimpleShader())
			return false;

		if (!CreateFurrShader())
			return false;

		return true;
	}

	bool CreateSimpleShader()
	{
		if (!simpleShader.Init())
			return false;

		if (!simpleShader.LoadShader("vert.glsl", GL_VERTEX_SHADER))
			return false;

		// if (!simpleShader.LoadShader("geom_simple.glsl", GL_GEOMETRY_SHADER))return false;

		//if (!simpleShader.LoadShader("tess_ctrl.glsl", GL_TESS_CONTROL_SHADER)) return false;
		//if (!simpleShader.LoadShader("tess_eval.glsl", GL_TESS_EVALUATION_SHADER)) return false;

		//if (!simpleShader.LoadShader("frag.glsl", GL_FRAGMENT_SHADER))return false;

		if (!simpleShader.LinkShaders())
			return false;

		return true;
	}

	bool CreateFurrShader()
	{
		if (!furShader.Init())
			return false;

		if (!furShader.LoadShader("vert.glsl", GL_VERTEX_SHADER)) return false;

		// if (!furShader.LoadShader("geom_furr.glsl", GL_GEOMETRY_SHADER)) return false;

		// if (!furShader.LoadShader("tess_ctrl.glsl", GL_TESS_CONTROL_SHADER)) return false;
		// if (!furShader.LoadShader("tess_eval.glsl", GL_TESS_EVALUATION_SHADER)) return false;

		if (!furShader.LoadShader("frag.glsl", GL_FRAGMENT_SHADER)) return false;

		if (!furShader.LinkShaders())
			return false;

		return true;
	}

	void RenderStart()
	{
		// First, render a square without any colors ( all vertexes will be black )
		// ===================
		// Make our background black
		glClearColor(0.5, 0.5, 0.5, 1.0);

		// We need to add GL_DEPTH_BUFFER_BIT so that the depth buffer will be cleared too.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	}

	void RenderModel(const Model &model)
	{
		model.Render( );
	}

	void RenderEnd()
	{
		// Swap our buffers to make our changes visible
		SDL_GL_SwapWindow(mainWindow);
	}

	void SetMatrix( const glm::mat4 &model )
	{
		glm::mat4 mvp = projection * view  * model;
		simpleShader.UseProgram();
		simpleShader.SetMatrix( mvp );

		furShader.UseProgram();
		furShader.SetMatrix( mvp );
	}

	void AddShaders(Model &m)
	{
		RenderPass simple;
		simple.shader = simpleShader;
		simple.renderMode = GL_TRIANGLES;
		simple.doRender = true;
		m.AddRenderPass(simple);

		RenderPass furr;
		furr.shader = furShader;
		// furr.renderMode = GL_PATCHES;
		furr.renderMode = GL_LINE_STRIP;
		// m.AddRenderPass(furr);
	}

	void Cleanup()
	{
		// Cleanup all the things we bound and allocated
		simpleShader.CleanUp();

		glDisableVertexAttribArray(0);

		// Delete our OpengL context
		SDL_GL_DeleteContext(mainContext);

		// Destroy our window
		SDL_DestroyWindow(mainWindow );

		// Shutdown SDL 2
		SDL_Quit();
	}

	void CheckSDLError(int line = -1)
	{
		std::string error = SDL_GetError();

		if (error != "")
		{
			std::cout << "SLD Error : " << error << std::endl;

			if (line != -1)
				std::cout << "\nLine : " << line << std::endl;

			SDL_ClearError();
		}
	}
	private:

	void SetOpenGLOptions()
	{
		// Enable blending so that we can have transparanet object
		glEnable(GL_BLEND ) ;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable depth testing so that closer triangles will hide the triangles farther away
		glEnable( GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);


		glEnable( GL_CULL_FACE);
	}

	bool SetSDL_GL_Attributes()
	{
		// Set our OpenGL version.
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

		// Turn on double buffering with a 24bit Z buffer.
		// You may need to change this to 16 or 32 for your system
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return true;
	}

	

	// Our SDL_Window ( just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;

	// Our opengl context handle
	SDL_GLContext mainContext;

	// Our wrapper to simplify the shader code
	Shader simpleShader;
	Shader furShader;

	glm::mat4 projection;

	// Camera matrix
	glm::mat4 view;
};




