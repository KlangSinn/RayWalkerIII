#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include <color.hpp>
#include "ray.hpp"
#include <glm/glm.hpp>

class Object {
	public:
		Object() {};		

		// MEHTOD FUNCTIONS --------------------------------------
		inline Color getColor() { return Color(0.0, 0.0, 0.0); };
		inline glm::vec3 getNormalAt(glm::vec3)	{ return glm::vec3(0, 0, 0); };
		inline double findIntersection(Ray ray) { return 0; }
		inline bool hasTexture() { return false; };
		inline double getOpacity() { return 1.0; };
		inline double getShineValue() { return 0.0; };
		inline glm::vec3 getLocalPoint(glm::vec3) { return glm::vec3(0.0, 0.0, 0.0); };
};

#endif
