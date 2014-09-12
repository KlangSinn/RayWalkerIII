#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "color.hpp"
#include <glm/glm.hpp>

class Source {
	public:
		Source() {};

		inline glm::vec3 getPosition() { return glm::vec3(0, 0, 0); }
		inline Color getColor() { return Color(1, 1, 1); }
};

#endif
