#include "EventHandler.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Math
{
	public:
		// Returns a simple axis which tells us which direction to move the object in
		static glm::vec3 GetAxis(const EventHandler &handler)
		{
			glm::vec3 axis(0.0f);

			if (handler.IsKeyDown(SDLK_UP))
				axis.y = -0.1f;

			if (handler.IsKeyDown(SDLK_DOWN))
				axis.y = 0.1f;

			if (handler.IsKeyDown(SDLK_LEFT))
				axis.x = -0.1f;

			if (handler.IsKeyDown(SDLK_RIGHT))
				axis.x = 0.1f;

			if (handler.IsKeyDown(SDLK_w))
				axis.z = -0.1f;

			if (handler.IsKeyDown(SDLK_s))
				axis.z = 0.1f;

			return axis;
		}

		static glm::vec3 GetScaleAxis(const EventHandler &handler)
		{
			glm::vec3 axis(1.0f);

			if (handler.IsKeyDown(SDLK_UP))
				axis.y = 1.01f;

			if (handler.IsKeyDown(SDLK_DOWN))
				axis.y = 0.99f;

			if (handler.IsKeyDown(SDLK_LEFT))
				axis.x = 1.01f;

			if (handler.IsKeyDown(SDLK_RIGHT))
				axis.x = 0.99f;

			if (handler.IsKeyDown(SDLK_w))
				axis.z = 1.01f;

			if (handler.IsKeyDown(SDLK_s))
				axis.z = 0.99;

			return axis;
		} 
		static glm::vec3 GetRotationAxis(const EventHandler &handler)
		{
			glm::vec3 axis = GetAxis(handler);

			return glm::vec3(axis.y, axis.x, axis.z);
		}
};
