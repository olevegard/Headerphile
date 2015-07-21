// Compile : clang++ main.cpp -o Part_02 -lGL -lSDL2
//
#include <iostream>
#include <string>

#include "Shader.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL2/SDL.h>

#include "../EventHandler.h"

#include "Renderer.h"
 
#define PROGRAM_NAME "Tutorial2"

bool runGame = true;

void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void Render();

EventHandler handler;
Renderer renderer;

glm::mat4 scale = glm::mat4(1.0f);  // Changes for each model !
glm::mat4 rotate = glm::mat4(1.0f);  // Changes for each model !
glm::mat4 translate = glm::mat4(1.0f);  // Changes for each model !

Model model;

bool Init()
{
	return renderer.Init();

}

void Update()
{
	handler.Update();

	runGame = !handler.WasQuit();


	model.Update(handler);

	/*
	glm::mat4 model;


	model = translate * rotate * scale;
	if ( handler.IsKeyDown( SDLK_r ))
	{
		translate = glm::mat4(1.0f);
		rotate = glm::mat4(1.0f);
		scale = glm::mat4(1.0f);

		model = glm::mat4(1.0f);
	}

	runGame = !handler.WasQuit();
	*/

	renderer.SetMatrix( model.GetModel() );

	Render();
}

int main(int argc, char *argv[])
{
	if (!Init())
		return -1;

	// Clear our buffer with a grey background
	// glClearColor(0.5, 0.5, 0.5, 1.0);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

	// SDL_GL_SwapWindow(mainWindow );

	std::cout << "Controls :"
		<< "\n\tRotate left : \t\t\t Left"
		<< "\n\tRotate right : \t\t\t Right"
		<< "\n\tRotate up : \t\t\t Up"
		<< "\n\tRotate down : \t\t\t Down"
		<< "\n\tRotate clockwise : \t\t w"
		<< "\n\tRotate counter-clockwise : \t s"
		<< std::endl;

	if (!model.SetupBufferObjects())
		return -1;

	renderer.AddShaders(model);

	while (runGame)
	{
		Update();
	}

	std::cout << "Rendering done!\n";

	renderer.Cleanup();

	return 0;
}

void Render()
{
	renderer.RenderStart();

	renderer.RenderModel(model);

	renderer.RenderEnd();
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

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

