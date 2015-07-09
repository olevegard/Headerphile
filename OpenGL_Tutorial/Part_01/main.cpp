<<<<<<< HEAD
// Headerphile.com OpenGL Tutorial part 1
// A Hello World in the world of OpenGL ( creating a simple windonw and setting background color )
// Source code is an C++ adaption / simplicication of : https://www.opengl.org/wiki/Tutorial1:_Creating_a_Cross_Platform_OpenGL_3.2_Context_in_SDL_(C_/_SDL)
// Compile : clang++ main.cpp -lGL -lSDL2 -std=c++11 -o Test
 
// C++ Headers
#include <string>
#include <iostream>
 
// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
 
// SDL2 Headers
#include <SDL2/SDL.h>
 
std::string programName = "Headerphile SDL2 - OpenGL thing";
 
// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;
 
// Our opengl context handle
SDL_GLContext mainContext;
 
bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void RunGame();
void Cleanup();

bool Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}
<<<<<<< HEAD
 
	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
 
	// Check that everything worked out okay
	if (!mainWindow)
=======

	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check that everything worked out okay
	if (!mainWindow )
>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}
<<<<<<< HEAD
 
	SetOpenGLAttributes();
 
	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);
 
	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);
 
	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	return true;
}
 
=======

	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow );

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	return true;
}

>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
<<<<<<< HEAD
 
	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
 
	return true;
}
 
=======

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
int main(int argc, char *argv[])
{
	if (!Init())
		return -1;
<<<<<<< HEAD
 
=======

>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
	// Clear our buffer with a black background
	// This is the same as :
	// 		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
	// 		SDL_RenderClear(&renderer);
	//
<<<<<<< HEAD
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainWindow);
 
	RunGame();
 
	Cleanup();
 
	return 0;
}
 
void RunGame()
{
	bool loop = true;
 
	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;
 
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_r:
					// Cover with red and update
					glClearColor(1.0, 0.0, 0.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					SDL_GL_SwapWindow(mainWindow);
					break;
				case SDLK_g:
					// Cover with green and update
					glClearColor(0.0, 1.0, 0.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					SDL_GL_SwapWindow(mainWindow);
					break;
				case SDLK_b:
					// Cover with blue and update
					glClearColor(0.0, 0.0, 1.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					SDL_GL_SwapWindow(mainWindow);
					break;
				default:
					break;
				}
			}
		}
 
		// Swap our back buffer to the front
		// This is the same as :
		// 		SDL_RenderPresent(&renderer);
	}
}
 
=======
	glClearColor ( 0.0, 0.0, 0.0, 1.0 );
	glClear ( GL_COLOR_BUFFER_BIT );
	SDL_GL_SwapWindow(mainWindow );

	RunGame();

	Cleanup();

	return 0;
}

void RunGame()
{
	bool loop = true;

	while ( loop )
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				loop = false;
			
			if ( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{
					case SDLK_ESCAPE:
						loop = false;
						break;
					case SDLK_r:
						// Cover with red and update
						glClearColor ( 1.0, 0.0, 0.0, 1.0 );
						glClear ( GL_COLOR_BUFFER_BIT );
						break;
					case SDLK_g:
						// Cover with green and update
						glClearColor ( 0.0, 1.0, 0.0, 1.0 );
						glClear ( GL_COLOR_BUFFER_BIT );
						break;
					case SDLK_b:
						// Cover with blue and update
						glClearColor ( 0.0, 0.0, 1.0, 1.0 );
						glClear ( GL_COLOR_BUFFER_BIT );
						break;
					default :
						break;
				}
			}
		}

		// Swap our back buffer to the front
		// This is the same as :
		// 		SDL_RenderPresent(&renderer);
		SDL_GL_SwapWindow(mainWindow);
	}
}

>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
void Cleanup()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);
<<<<<<< HEAD
 
	// Destroy our window
	SDL_DestroyWindow(mainWindow);
 
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
 
=======

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

>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;
<<<<<<< HEAD
 
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
=======

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
>>>>>>> 8848344fff41fbe1a2e82b6bcebcdf0333d51b7c
