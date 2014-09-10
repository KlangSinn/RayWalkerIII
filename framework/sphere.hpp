#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include "color.hpp"
#include "object.hpp"
#include <glm/glm.hpp>

class Sphere : public Object {
public:
		Sphere();
		Sphere( glm::ivec3, double, Color, double, double);

		// method functions ------------------------------------------
		
		double getOpacity() { return opacity_; };

		double getShineValue() { return shine_; };
				
		Color 	getColor();
		glm::ivec3 getNormalAt(glm::ivec3 intersection_position);
		double findIntersection(Ray ray);

		glm::ivec3 getLocalPoint(glm::ivec3 hitPoint) {
			return glm::ivec3 (hitPoint[0] - center[0], hitPoint[1] - center[1],hitPoint[2] - center[2]);
		};

		glm::ivec3 	getSphereCenter() 	{ return center; }
		double 	getSphereRadius() 	{ return radius; }
private:
	glm::ivec3 center;
	double radius;
	Color color;
	double opacity_;
	double shine_;
};

#endif
