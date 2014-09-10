#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "color.hpp"
#include "ray.hpp"
#include <glm/glm.hpp>

class Object {
	public:
		Object();		

		// MEHTOD FUNCTIONS --------------------------------------
		virtual Color getColor() { return Color(0.0, 0.0, 0.0); };
		virtual glm::ivec3 getNormalAt(glm::ivec3 intersection_position)	{ return glm::ivec3(0, 0, 0); }
		virtual double findIntersection(Ray ray) { return 0; }
		virtual bool hasTexture() { return false; };
		virtual double getOpacity() { return 1.0; };
		virtual double getShineValue() { return 0.0; };
		virtual glm::ivec3 getLocalPoint(glm::ivec3 hitPoint) { return glm::ivec3(0.0, 0.0, 0.0); };
};

Object::Object() {}

#endif
