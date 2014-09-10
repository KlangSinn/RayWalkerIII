#include "sphere.hpp"
#include "object.hpp"

Color Sphere::getColor() 	{ 
	return color; 
}

glm::ivec3 Sphere::getNormalAt(glm::ivec3 point) {
			
	// normal always points away from the center of a sphere
	// line between center of sphere and the given point is the normal vector
	glm::ivec3 normalvector(point - center);
	return normalvector;
}

// Returns distance from ray origin to point of intersection
// if no intersection was found, funtion return -1
double Sphere::findIntersection(Ray ray) {
	
	// GET POINTS AND VECTORS
	glm::ivec3 ray_origin = ray.getRayOrigin();
	glm::ivec3 ray_direction = ray.getRayDirection();

	double a = 1; // normalized
	double b =  (2 * (ray_origin[0] - center[0]) * ray_direction[0]) + 
				(2 * (ray_origin[1] - center[1]) * ray_direction[1]) + 
				(2 * (ray_origin[2] - center[2]) * ray_direction[2]);
	double c =  pow(ray_origin[0] - center[0], 2) + 
				pow(ray_origin[1] - center[1], 2) + 
				pow(ray_origin[2] - center[2], 2) - 
				(radius * radius);
	double discriminant = b * b - 4 * c;

	if(discriminant > 0) {
				
		// the ray intersects the sphere
		// but there can be two intersections
		// first root
		double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

		if(root_1 > 0) {
					
			// the first root is the smallest positive root
			return root_1;	
		} else {

			// the second root is the smallest positive root
			double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
			return root_2;
		} 
	} else {

		// the ray missed the sphere
		return -1;
	}
}

Sphere::Sphere() {
	center = glm::ivec3(0, 0, 0);
	radius = 1.0;
	color  = Color(0.5, 0.5, 0.5);
	shine_ = 0.0;
}

Sphere::Sphere(glm::ivec3 centerValue, double radiusValue, Color colorValue, double opa, double shine) {
	center = centerValue;
	radius = radiusValue;
	color  = colorValue;
	opacity_ = opa;
	shine_ = shine;
}