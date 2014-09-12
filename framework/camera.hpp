#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/glm.hpp>

class Camera {

	// vectors defining position and direction of camera
	glm::vec3 camdir; 
	glm::vec3 camright; 
	glm::vec3 camdown;
	
	glm::vec3 campos;

	public:
		Camera();
		Camera( glm::vec3, glm::vec3, glm::vec3, glm::vec3 );

		// method functions
		glm::vec3 getCameraPosition() { return campos; }
		glm::vec3 getCameraDirection() { return camdir; }
		glm::vec3 getCameraRight() { return camright; }
		glm::vec3 getCameraDown() { return camdown; }
};

#endif
