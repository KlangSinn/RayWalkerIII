#ifndef _RAY_HPP
#define _RAY_HPP

#include <glm/glm.hpp>

class Ray {

	// two vectors define a ray
	// origin and direction
	glm::ivec3 origin; 
	glm::ivec3 direction;

	public:
		Ray();
		Ray( glm::ivec3, glm::ivec3 );

		// method functions
		glm::ivec3 getRayOrigin() { return origin; }
		glm::ivec3 getRayDirection() { return direction; }
};

Ray::Ray() {
	origin 		= glm::ivec3(0, 0, 0);
	direction 	= glm::ivec3(1, 0, 0);
}

Ray::Ray(glm::ivec3 o, glm::ivec3 d) {
	origin 		= o;
	direction 	= d;
}

#endif
