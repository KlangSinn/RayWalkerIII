#ifndef _PLANE_HPP
#define _PLANE_HPP

#include "color.hpp"
#include "object.hpp"
#include <glm/glm.hpp>
 
class Plane : public Object {
	glm::ivec3 normal;
	double distance;
	Color color;

	public:
		Plane();
		Plane( glm::ivec3, double, Color );

		// METHOD FUNCTIONS ----------------------------

		glm::ivec3 	getPlaneNormal() 	{ return normal; }
		double 	getPlaneDistance() 	{ return distance; }
		Color 	getColor() 	{ return color; }

		// Normal at point of intersecting 
		// For planes it is the normal normal :D
		glm::ivec3 getNormalAt(glm::ivec3 point)  {
			return normal;
		}

		// Intersection of a ray and the object
		double findIntersection(Ray ray) {
			glm::ivec3 ray_direction = ray.getRayDirection();
			double a = glm::dot(ray_direction, normal);

			if(a == 0) {
				
				// ray is parallel to the plane -> will never interesct it
				return -1;
			} else {

				// calculate distance from the camera on ray				
				double b = glm::dot(normal, (glm::ivec3(ray.getRayOrigin()[0], ray.getRayOrigin()[1], ray.getRayOrigin()[2]) + glm::ivec3(normal[0] * distance, normal[1] * distance, normal[2] * distance) * (-1)));
				return -1 * b / a;
			}
		}
};

Plane::Plane() {
	normal = glm::ivec3(1, 0, 0);
	distance = 0;
	color  = Color(0.5, 0.5, 0.5);
}

Plane::Plane(glm::ivec3 normalValue, double distanceValue, Color colorValue) {
	normal = normalValue;
	distance = distanceValue;	
	color  = colorValue;
}

#endif
