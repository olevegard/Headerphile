#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <vector>

#include "Shader.h"
#include "../EventHandler.h"

struct RenderPass
{
	GLenum renderMode;
	Shader shader;
	bool doRender;

	RenderPass()
		:	doRender(true)
	{

	}
};

class Model
{
	public: 

	Model()
		:	positionAttributeIndex(0)
		,	colorAttributeIndex(1)
	{
	}

	glm::mat4 GetModel()
	{
		return model;
	}

	void SetMatrix(glm::mat4 mvp)
	{
		shader.UseProgram();
		shader.SetMatrix( mvp );
	}

	void Render() const
	{
		shader.UseProgram();
		glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3 );
	}

	void Update(const EventHandler &handler)
	{
		if (handler.IsKeyDown(SDLK_r))
		{
			ResetMatrix();
			return;
		}

		HandleMoveKeys(handler);

		UpdateMatrix();
	}

	void ResetMatrix()
	{
		rotate = glm::mat4(1.0f); 
		scale = glm::mat4(1.0f); 
		translate = glm::mat4(1.0f); 

		model = glm::mat4(1.0f); 
	}

	void HandleMoveKeys(const EventHandler &handler)
	{
		glm::vec3 axis = GetAxis(handler);

		if ( axis == glm::vec3(0.0f, 0.0f, 0.0f ))
			return;

		if (handler.IsKeyDown(SDLK_LSHIFT) || handler.IsKeyDown(SDLK_RSHIFT) )
		{
			glm::vec3 rotAxis = glm::vec3(axis.y, axis.x, axis.z); 

			rotate *= glm::rotate( 0.1f, rotAxis);
		}
		else if (handler.IsKeyDown(SDLK_LCTRL) || handler.IsKeyDown(SDLK_RCTRL) )
		{
			if (handler.IsKeyDown(SDLK_LCTRL))
				axis *= 1.01;
			else
				axis *= 0.99;

			if (axis.x == 0.0f)
				axis.x = 1.0f;

			if (axis.y == 0.0f)
				axis.y = 1.0f;

			if (axis.z == 0.0f)
				axis.z = 1.0f;

			scale = glm::scale(scale, axis);
		}
		else
		{
			axis *= 0.1;
			translate *= glm::translate(axis);
		}
	}

	void UpdateMatrix()
	{
		model = translate * rotate * scale;
	}

	static glm::vec3 GetAxis(const EventHandler &handler)
	{
		glm::vec3 axis;
		if (handler.IsKeyDown(SDLK_UP))
			axis.y = -1.0f;

		if (handler.IsKeyDown(SDLK_DOWN))
			axis.y = 1.0f;

		if (handler.IsKeyDown(SDLK_LEFT))
			axis.x = -1.0f;

		if (handler.IsKeyDown(SDLK_RIGHT))
			axis.x = 1.0f;

		if (handler.IsKeyDown(SDLK_w))
			axis.z = -1.0f;

		if (handler.IsKeyDown(SDLK_s))
			axis.z = 1.0f;

		return axis;
	}

	void CleanUp()
	{
		// Cleanup all the things we bound and allocated
		shader.CleanUp();

		glDisableVertexAttribArray(0);
		glDeleteBuffers(1, vbo);
		glDeleteVertexArrays(1, vao);

		// Shutdown SDL 2
		SDL_Quit();
	}

	bool SetupBufferObjects()
	{
		positions = ReadFile("positions.txt");
		colors = ReadFile("colors.txt");

		// Generate and assign two Vertex Buffer Objects to our handle
		glGenBuffers(2, vbo);

		// Generate and assign a Vertex Array Object to our handle
		glGenVertexArrays(1, vao);

		// Bind our Vertex Array Object as the current used object
		glBindVertexArray(vao[0]);

		// Colors
		// =======================
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

		// Copy the vertex data from diamond to our buffer
		glBufferData(GL_ARRAY_BUFFER,  colors.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);

		// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
		glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Positions
		// ===================
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

		// Copy the vertex data from diamond to our buffer
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);

		// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
		glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0 );

		// Enable our attributes within the current VAO
		glEnableVertexAttribArray(positionAttributeIndex);
		glEnableVertexAttribArray(colorAttributeIndex);

		// Set up shader 
		// ===================
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(vao[0]);

		return true;
	}

	static std::vector<GLfloat> ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);

		std::vector<GLfloat> result;
		std::cout << "Reading : " << file << std::endl;

		while (t.good())
		{
			std::string str;
			t >> str;

			GLfloat f = std::atof(str.c_str());

			result.push_back(f);
		}

		return result;
	}

	void SetShader(Shader _shader)
	{
		shader = _shader;
	}

	private:

	// VBO data
	std::vector<GLfloat> positions;
	std::vector<GLfloat> colors;


	// Create variables for storing the ID of our VAO and VBO
	GLuint vbo[2], vao[1]; 

	// The positons of the position and color data within the VAO
	const uint32_t positionAttributeIndex, colorAttributeIndex;

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model;
	glm::mat4 translate;
	glm::mat4 scale;
	glm::mat4 rotate;

	Shader shader;
};
