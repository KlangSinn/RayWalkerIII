#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include "color.hpp"
#include "source.hpp"
#include <glm/glm.hpp>

class Light : public Source {
	glm::ivec3 position;
	Color color;

	public:
		Light();
		Light( glm::ivec3, Color );

		// method functions
		glm::ivec3 getPosition() { return position; }
		Color getColor() { return color; }
};

Light::Light() {
	position = glm::ivec3(0, 0, 0);
	color 	 = Color(1.0, 1.0, 1.0);
}

Light::Light(glm::ivec3 p, Color c) {
	position = p;
	color 	 = c;
}

#endif
