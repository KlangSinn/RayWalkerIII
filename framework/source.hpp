#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "color.hpp"
#include <glm/glm.hpp>

class Source {
	public:
		Source();

		virtual glm::ivec3 getPosition() { return glm::ivec3(0, 0, 0); }
		virtual Color getColor() { return Color(1, 1, 1); }
};

Source::Source() {}

#endif
