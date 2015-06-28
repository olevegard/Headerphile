// Compile : clang++ main.cpp -o Part_02 -lGL -lSDL2
//
#include <iostream>
#include <string>

#include "Shader.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
 
#define PROGRAM_NAME "Tutorial2"

std::string programName = "Headerphile SDL2 - OpenGL thing";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

// Our object has 4 points, each point has 3 values
const uint32_t points = 4;
const uint32_t floatsPerPoint = 3;
const uint32_t positionAttributeIndex = 0;

// This is the object we'll draw ( a simple square
const GLfloat diamond[points][floatsPerPoint] = {
	{ -0.5,  0.5,  0.5 }, // Top left
	{  0.5,  0.5,  0.5 }, // Top right
	{  0.5, -0.5,  0.5 }, // Bottom right 
	{ -0.5, -0.5,  0.5 }, // Bottom left
};

// Create variables for storing the ID of our VAO and VBO
GLuint vbo[1], vao[1]; 

// Our wrapper to simplify the shader code
Shader shader;

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);

void Render()
{
	for (uint32_t i=2; i <= points; i++)
	{
		/* Make our background black */
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes */
		glDrawArrays(GL_LINE_LOOP, 0, i);

		/* Swap our buffers to make our changes visible */
		SDL_GL_SwapWindow(mainWindow);

		std::cout << "Press ENTER to render next frame\n";
		std::cin.ignore();
	}
}
void SetupBufferObjects()
{
	// Set up our VBO
	// ===================
	// Allocate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(1, vbo);

	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

	// Set up our VAO
	// ===================
	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);

	// Set up shader ( will be covered in the next part )
	// ===================
	shader.Init();

	shader.BindAttributeLocation(0, "in_Position");
	shader.UseProgram();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow );

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	glewExperimental = GL_TRUE; 
	glewInit();
	return true;
}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void Cleanup()
{
	// Cleanup all the things we bound and allocated
	shader.CleanUp();

	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);

	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow );

	// Shutdown SDL 2
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	if (!Init())
		return -1;

	// Clear our buffer with a black background
	glClearColor ( 0.0, 0.0, 0.0, 1.0 );
	glClear ( GL_COLOR_BUFFER_BIT );

	SDL_GL_SwapWindow(mainWindow );

	std::cout << "Seting up VBO + VAO..." << std::endl;
	SetupBufferObjects();
	std::cout << "Rendering..." << std::endl;
	Render();

	std::cout << "Rendering done!\n";
	std::cin.ignore();

	Cleanup();

	return 0;
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

