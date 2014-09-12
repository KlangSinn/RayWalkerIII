// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, std::vector<Object*> scene_objects, std::vector<Source*> source_objects, double aspectratio, double accuracy, double ambientlight, Camera scenecam)
  : width_(w)
  , height_(h)
  , colorbuffer_(w*h, Color(0.0, 0.0, 0.0))
  , filename_(file)
  , ppm_(width_, height_)
  , scene_objects_(scene_objects)
  , source_objects_(source_objects)
  , aspectratio_(aspectratio)
  , accuracy_(accuracy)
  , scenecam_(scenecam)
  , ambientlight_(ambientlight)
{}

void Renderer::render() {

	// GO THROUGH PIXELS
	//for (unsigned y = 0; y < height_; ++y) {
	for (unsigned y = height_ - 1; y > 0; --y) {
		for (unsigned x = 0; x < width_; ++x) {
			Pixel p(x, y);			
				
			// start with no anti-aliasing
			if(width_ > height_) {

				// the image is wider than it is tall
				xamnt_ = ((x + 0.5) / width_) * aspectratio_ - (((width_ - height_) / (double) height_) / 2);
				yamnt_ = ((height_ - y) + 0.5) / height_;				
			} else if(height_ > width_) {

				// the image is taller than it is wide
				xamnt_ = (x + 0.5) / width_;
				yamnt_ = (((height_ - y) + 0.5) / height_) / aspectratio_ - (((height_ - width_) / (double) width_) / 2);
			} else {

				// the image is square
				xamnt_ = (x + 0.5) / width_;
				yamnt_ = ((height_ - y) + 0.5) / height_;
			}

			// create ray from camera through pixel into scene
			// camir + (camright * (xamnt - 0.5) + camdown * (yamnt - 0.5))
			glm::vec3 cam_ray_direction = camdir_ + (camright_ * (float) (xamnt_ - 0.5) + (camdown_ * (float) (yamnt_ - 0.5)));
			glm::normalize(cam_ray_direction);
			Ray cam_ray (campos_, cam_ray_direction);

			std::vector<double> intersections;

			// loop over all objects and look if they intersect with the ray
			for(int index = 0; index < scene_objects_.size(); index++) {
				intersections.push_back(scene_objects_.at(index)->findIntersection(cam_ray));
			}

			std::cout << intersections.size() << std::endl;

			// which objects is closest to the camera
			int index_of_winning_object = winningObjectIndex(intersections);

			if(index_of_winning_object == -1) {

				// background color
				// make black because it missed all objects
				p.color = Color(255, 0, 0);
			} else {

				// if it hits an object, get color of object
				// accuracy ???
				if(intersections.at(index_of_winning_object) > accuracy_) {
					glm::vec3 intersection_position = campos_ + (cam_ray_direction * (float) (intersections.at(index_of_winning_object)));
					glm::vec3 intersecting_ray_direction = cam_ray_direction;
					Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, index_of_winning_object);
					p.color = Color(intersection_color);
				}
			}	

			write(p); 
		}
	}
	/*
	CHECKBOARD
	const std::size_t checkersize = 10;

	for (unsigned y = 0; y < height_; ++y) {
	for (unsigned x = 0; x < width_; ++x) {
		Pixel p(x,y);
		if ( ((x/checkersize)%2) != ((y/checkersize)%2)) {
		p.color = Color(0.0, 1.0, float(x)/height_);
		} else {
		p.color = Color(1.0, 0.0, float(y)/width_);
		}

		write(p);
	}
	}
	for (unsigned y = 0; y < height_; ++y) {
		for (unsigned x = 0; x < width_; ++x) {
			Pixel p(x,y);
			float color_value = float(y) / width_;
			p.color = Color(color_value, color_value, color_value);			

			write(p);
		}
	}*/
	ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= colorbuffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    colorbuffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}

int Renderer::winningObjectIndex(std::vector<double> object_intersections) {

	// return index of the winning intersection
	// which object intersected by the ray is closer to the camera
	int index_of_minimum_value;

	// prevent unnessary calculations
	// if there are no intersections
	if(object_intersections.size() == 0) {
		return -1;
	} else if(object_intersections.size() == 1) {
		if(object_intersections.at(0) > 0) {

			// is that intersetion is greater than zero then its our index of minimum value
			// return index-> with one object the index is 0
			return 0;
		} else {

			// otherwise intersection value is negative
			return -1;
		}
	} else {

		// more than one intersection was found
		// find maximimum
		double max = 0;
		for(int i = 0; i < object_intersections.size(); i++) {
			if(max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		// then starting from the maximum valze find the minimum positive value
		if(max > 0) {
			
			// we only want positive intersections
			for(int index = 0; index < object_intersections.size(); index++) {
				if(object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}
			return index_of_minimum_value;
		} else {

			// all intersections were negative
			return -1;
		}
	}
}

Color Renderer::getColorAt(glm::vec3 intersection_position, glm::vec3 intersecting_ray_direction, int index_of_winning_object) {	
	
		
		// OBJECT COLOR -----------------------------------------------------------------------------------------
		Color winning_object_color;

		/*// HAS THE OBJECT A TEXTURE?
		if(scene_objects_.at(index_of_winning_object)->hasTexture()) {
			Texture objtext = scene_objects_.at(index_of_winning_object)->getMaterial();

			// calculate local hit point on sphere for texture mapping		
			winning_object_color = objtext.get_color(scene_objects_.at(index_of_winning_object)->getLocalPoint(intersection_position).normalize());
		} else {

			// ELSE SIMPLY GET COLOR OF OBJECT
			winning_object_color = scene_objects_.at(index_of_winning_object)->getColor();
		}		*/

		winning_object_color = scene_objects_.at(index_of_winning_object)->getColor();

		// normal to compare with ray to light source
		glm::vec3 winning_object_normal = scene_objects_.at(index_of_winning_object)->getNormalAt(intersection_position);

		// WAY TO MAKE SQUARED SURFACE --------------------------------------------------------------------------
		//if(winning_object_color.getColorSpecial() == 2) {

			// cause of this int values the squares are bigger than one pixel
			// checkered/tile floor pattern
			/*int square = (int) floor(intersection_position[0]) + (int) floor (intersection_position[2]);

			if((square % 2) == 0) {

				// dark tile
				winning_object_color = winning_object_color.scalar(0.1);
			} */
		//}

		// if object is in shadow final color is calculated by objects color and ambient light
		Color final_color = Color(winning_object_color.r * ambientlight_, winning_object_color.g * ambientlight_, winning_object_color.b * ambientlight_);		

		// REFLECTIONS ------------------------------------------------------------------------------------------
		// reflection of object with specular intensity
		double shineValue = scene_objects_.at(index_of_winning_object)->getShineValue();
		if(shineValue > 0 && shineValue <= 1) {
			glm::vec3 intersecting_ray_direction_negative = intersecting_ray_direction;
			intersecting_ray_direction_negative *= (-1);
			double dot1 = glm::dot(intersecting_ray_direction_negative, winning_object_normal);
			glm::vec3 scalar1 = winning_object_normal;
			scalar1 *= dot1;
			glm::vec3 add1 = operator+(scalar1, intersecting_ray_direction);
			glm::vec3 scalar2 = add1;
			scalar2 *= 2;
			glm::vec3 add2 = intersecting_ray_direction_negative + (scalar2);
			glm::vec3 reflection_direction = glm::normalize(add2);

			Ray reflection_ray(intersection_position, reflection_direction);

			// determine what the ray intersects with first
			std::vector<double> reflection_intersections;

			for(int reflection_index = 0; reflection_index < scene_objects_.size(); reflection_index++) {
				reflection_intersections.push_back(scene_objects_.at(reflection_index)->findIntersection(reflection_ray));			
			}

			int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

			if(index_of_winning_object_with_reflection != -1) {
				// reflection ray missed everything
				if(reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy_) {
					// determine the position and direction at the point of intersection with the reflection
					// the ray only affects the color if it reflected off something

					glm::vec3 reflection_intersection_position = intersection_position + reflection_direction * (float)(reflection_intersections.at(index_of_winning_object_with_reflection));
					glm::vec3 reflection_intersecting_ray_direction = reflection_direction;

					Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersecting_ray_direction, index_of_winning_object_with_reflection);				

					reflection_intersection_color.r *= shineValue;
					reflection_intersection_color.g *= shineValue;
					reflection_intersection_color.b *= shineValue;
					final_color = final_color + reflection_intersection_color;

				}
			}
		}

		// SHADOWS ----------------------------------------------------------------------------------------------
		/*for(int light_index = 0; light_index < source_objects_.size(); light_index++) {			
			glm::vec3 light_direction(intersection_position, source_objects_.at(light_index)->getPosition());			
			glm::vec3 light_direction_norm = light_direction;
			light_direction_norm.normalize();

			float cosine_angle = dot(winning_object_normal, light_direction_norm);
			if(cosine_angle > 0) {				
			
				// test for shadows, cause normal of surface is in direction to the light source
				bool shadowed = false;

				// ray from intersetion point to light source
				// check for intersections, if true than is in shadow
				float distance_to_light_magnitude = light_direction.magnitude();
				Ray shadow_ray(intersection_position, light_direction_norm);
				std::vector<double> secondary_intersections;
				for(int object_index = 0; object_index < scene_objects_.size(); object_index++) {
					secondary_intersections.push_back(scene_objects_.at(object_index)->findIntersection(shadow_ray));
				}
				for(int c = 0; c < secondary_intersections.size() && shadowed == false; c++) {
					if(secondary_intersections.at(c) > accuracy_) {
						if(secondary_intersections.at(c) <= distance_to_light_magnitude) {
							shadowed = true;
							break;
						}					
					}				
				}
				if(shadowed == false) {
					final_color = final_color.colorAdd(winning_object_color.colorMultiply(source_objects_.at(light_index)->getColor()).scalar(cosine_angle));
					//final_color = final_color.colorAdd(final_color.colorMultiply(source_objects_.at(light_index)->getColor()).scalar(cosine_angle));

					// add some shine special between 0 and 1
					if(shineValue > 0 && shineValue <= 1) {
						double dot1 = dot(winning_object_normal, intersecting_ray_direction.negative());
						glm::vec3 scalar1 = winning_object_normal * dot1;
						glm::vec3 add1 = scalar1 + (intersecting_ray_direction);
						glm::vec3 scalar2 = add1 * 2;
						glm::vec3 add2 = intersecting_ray_direction.negative() + (scalar2);
						glm::vec3 reflection_direction = add2.normalize();

						double specular = dot(reflection_direction, light_direction_norm);
						if(specular > 0) {
							specular = pow(specular, 10);
							//std::cout << specular * shineValue << " ";
							final_color = final_color.colorAdd(source_objects_.at(light_index)->getColor().scalar(specular * shineValue));
						}
					}
				}				
			} 

		}

		// OPACITY -----------------------------------------------------------------------------------------------
		if(scene_objects_.at(index_of_winning_object)->getOpacity() < 1) {
			Ray opa_ray(intersection_position, intersecting_ray_direction);
			std::vector<double> opa_intersections;
			for(int opa_index = 0; opa_index < scene_objects_.size(); opa_index++) {
				if(opa_index != index_of_winning_object)
					opa_intersections.push_back(scene_objects_.at(opa_index)->findIntersection(opa_ray));
				else
					opa_intersections.push_back(-1);
			}
			int index_of_winning_object_with_opa = winningObjectIndex(opa_intersections);
			if(index_of_winning_object_with_opa != -1) {
				if(opa_intersections.at(index_of_winning_object_with_opa) > accuracy_) {
					glm::vec3 opa_intersection_position = operator+(intersection_position, (operator*(opa_intersections.at(index_of_winning_object_with_opa), intersecting_ray_direction)));
					glm::vec3 opa_intersecting_ray_direction = intersecting_ray_direction;

					Color opa_intersection_color = getColorAt(opa_intersection_position, opa_intersecting_ray_direction, index_of_winning_object_with_opa);									
					double opa = scene_objects_.at(index_of_winning_object)->getOpacity();
					double nopa = 1 - opa;
					final_color = final_color.scalar(opa).colorAdd(opa_intersection_color.scalar(nopa));
					//final_color = final_color.colorAdd((Color(-1, -1, -1).colorAdd(opa_intersection_color)).scalar(scene_objects_.at(index_of_winning_object)->getOpacity()));
				}
			} else {				
				//final_color = final_color.colorAdd((Color(-1, -1, -1).colorAdd(Color(0,0,0))).scalar(scene_objects_.at(index_of_winning_object)->getOpacity()));
				final_color = final_color.scalar(scene_objects_.at(index_of_winning_object)->getOpacity()).colorAdd(Color(0,0,0).scalar(1 - scene_objects_.at(index_of_winning_object)->getOpacity()));
			}
		}*/

		return final_color.clip();
	}