#ifndef _CAMERA_HPP
#define _CAMERA_HPP
#include <glm/glm.hpp>

class Camera {

	// vectors defining position and direction of camera
	glm::ivec3 camdir; 
	glm::ivec3 camright; 
	glm::ivec3 camdown;
	
	glm::ivec3 campos;

	public:
		Camera();
		Camera( glm::ivec3, glm::ivec3, glm::ivec3, glm::ivec3 );

		// method functions
		glm::ivec3 getCameraPosition() { return campos; }
		glm::ivec3 getCameraDirection() { return camdir; }
		glm::ivec3 getCameraRight() { return camright; }
		glm::ivec3 getCameraDown() { return camdown; }
};

#endif
