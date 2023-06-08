#pragma once

//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <sstream>
#include <iomanip>



namespace Velvet
{
	namespace Helper
	{
		inline std::string to_string(const glm::vec3& p) {
			std::stringstream ss;
			ss << "[" << std::fixed << std::setprecision(2) << p.x << ", "
				<< std::fixed << std::setprecision(2) << p.y << ", "
				<< std::fixed << std::setprecision(2) << p.z << "]";
			return ss.str();
		}


		inline std::string to_string(const glm::vec2& p) {
			std::stringstream ss;
			ss << "[" << std::fixed << std::setprecision(2) << p.x << ", "
				<< std::fixed << std::setprecision(2) << p.y << "]";
			return ss.str();
		}

		glm::mat4 RotateWithDegree(glm::mat4 result, const glm::vec3& rotation);

		glm::vec3 RotateWithDegree(glm::vec3 result, const glm::vec3& rotation);

		float Random(float min = 0, float max = 1);

		glm::vec3 RandomUnitVector();

		template <class T>
		T Lerp(T value1, T value2, float a)
		{
			a = min(max(a, 0.0f), 1.0f);
			return a * value2 + (1 - a) * value1;
		}
	}
}