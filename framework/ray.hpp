#ifndef _RAY_HPP
#define _RAY_HPP

#include <glm/glm.hpp>

class Ray {
public:
	Ray() {
		origin_		= glm::vec3(0, 0, 0);
		direction_ 	= glm::vec3(1, 0, 0);
	};
	Ray(glm::vec3 origin, glm::vec3 direction) {
		origin_ 	= origin;
		direction_ 	= direction;
	};

	// method functions
	inline glm::vec3 getRayOrigin() { return origin_; }
	inline glm::vec3 getRayDirection() { return direction_; }

private:
	// two vectors define a ray
	// origin and direction
	glm::vec3 origin_; 
	glm::vec3 direction_;
};

#endif
