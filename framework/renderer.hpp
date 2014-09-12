// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "object.hpp"
#include "source.hpp"
#include "camera.hpp"
#include "ppmwriter.hpp"
#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:
	//Renderer(unsigned w, unsigned h, std::string const& file);
	Renderer(unsigned w, unsigned h, std::string const& file, std::vector<Object*> scene_objects, std::vector<Source*> source_objects, double aspectratio, double accuracy, double ambientlight, Camera scenecam);
	void render();
	void write(Pixel const& p);
	int winningObjectIndex(std::vector<double> object_intersections);
	Color getColorAt(glm::vec3 intersection_position, glm::vec3 intersecting_ray_direction, int index_of_winning_object);

	inline std::vector<Color> const& colorbuffer() const
	{
	return colorbuffer_;
	}

private:
	unsigned width_;
	unsigned height_;
	std::vector<Color> colorbuffer_;
	std::string filename_;
	PpmWriter ppm_;

	// SCENE
	std::vector<Object*> scene_objects_;
	std::vector<Source*> source_objects_;
	double	aspectratio_;
	double	accuracy_;
	Camera	scenecam_;
	double xamnt_, yamnt_;
	glm::vec3 camdir_;
	glm::vec3 campos_;
	glm::vec3 camdown_;
	glm::vec3 camright_;
	double ambientlight_;	
};


#endif // #ifndef BUW_RENDERER_HPP
