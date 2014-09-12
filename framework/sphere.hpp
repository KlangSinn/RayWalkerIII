#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include "object.hpp"
#include "math.h"
#include "ray.hpp"
#include "color.hpp"
#include <glm/glm.hpp>

class Sphere : public Object {
public:
		Sphere();
		Sphere( glm::vec3, double, Color, double, double);

		// METHOD FUNCTIONS ///////////////////////////////////////////////////////////////////////////////				
		Color 	getColor();
		glm::vec3 getNormalAt(glm::vec3 intersection_position);
		double findIntersection(Ray ray);

		inline glm::vec3 getLocalPoint(glm::vec3 hitPoint) {
			return glm::vec3 (hitPoint[0] - center[0], hitPoint[1] - center[1],hitPoint[2] - center[2]);
		};
		inline double getOpacity() { return opacity_; };
		inline double getShineValue() { return shine_; };
		inline glm::vec3 	getSphereCenter() 	{ return center; };
		inline double 	getSphereRadius() 	{ return radius; };
private:
	glm::vec3 center;
	double radius;
	Color color;
	double opacity_;
	double shine_;
};

#endif
