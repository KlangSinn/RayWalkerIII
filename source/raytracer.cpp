#include <thread>
#include <renderer.hpp>
#include <fensterchen.hpp>

int main(int argc, char* argv[])
{

	// CREATE VECTORS AND POINTS ----------------------------------------
	glm::vec3 origin(0, 0, 0);	
	glm::vec3 x(1, 0, 0);
	glm::vec3 y(0, 1, 0);
	glm::vec3 z(0, 0, 1);

	glm::vec3 p1(3, 3, -6);
	glm::vec3 p2(-1, -0.5, -5);
	glm::vec3 p3(2, 0, -3);

	// CAMERA -----------------------------------------------------------
	glm::vec3 campos(0, 0, 5);
	glm::vec3 lookat(0, 0, -1);

	glm::vec3 camdir(lookat - campos); 
	glm::vec3 camright = glm::cross(camdir, y);
	glm::vec3 camdown = glm::cross(camdir, camright);

	glm::normalize(camdir);
	glm::normalize(camright);
	glm::normalize(camdown);

	Camera scenecam(campos, camdir, camright, camdown);

	// COLORS -----------------------------------------------------------
	Color red(1.0, 0.0, 0.0);
	Color blue(0.0, 0.0, 1.0);
	Color violet(1.0, 0.0, 1.0);
	Color yellow(1.0, 1.0, 0.0);
	Color cyan(0.0, 1.0, 1.0);
	Color green(0.0, 1.0, 0.0);
	Color orange(1.0, 0.5, 0.0);
	Color white(1.0, 1.0, 1.0);
	Color black(0.0, 0.0, 0.0);
	Color gray(0.5, 0.5, 0.5);

	// LIGHT ------------------------------------------------------------
	double ambientlight = 0.4;
	glm::vec3 light_position(5, 5, 10);
	Light scene_light(light_position, white);
	Light scene_light2(light_position, white);

	// vector for all light sources
	std::vector<Source*> sources;
	sources.push_back(dynamic_cast<Source*>(&scene_light));
	//sources.push_back(dynamic_cast<Source*>(&scene_light2));

	// OBJECTS ----------------------------------------------------------
	
	// sceneobject to hold all objects in the scene that we can index them
	// important to loop over them to find intersections

	std::vector<Object*> scene_objects;	

	Sphere scene_sphere(p2, 1.5, cyan, 1.0, 0.0);
	Sphere scene_sphere3(p2, 1.8, white, 0.05, 0.0);

	Sphere scene_sphere1(p3, 0.7, green, 0.5, 0.2);	
	Sphere scene_sphere2(p1, 0.8, red, 1.0, 0.5);	
	
	Plane plane(y, -7, gray);

	// SCENE ------------------------------------------------------------
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere1));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
	scene_objects.push_back(dynamic_cast<Object*>(&plane));

	double accuracy 	= 0.000001;

	// WINDOW RESOLUTION
	unsigned const width = 600;
	unsigned const height = 600;
	const double aspectratio = (double) width / (double) height;

	// OUTPUT -----------------------------------------------------------
	std::cout << "Hello, I am RayWalker." << std::endl;	
	std::string const filename = "./raywalker.ppm";

	Renderer app(width, height, filename, scene_objects, sources, aspectratio, accuracy, ambientlight, scenecam);

	std::thread thr([&app]() { app.render(); });

	Window win(glm::ivec2(width,height));

	while (!win.shouldClose()) {
	    if (win.isKeyPressed(GLFW_KEY_ESCAPE)) {
		win.stop();
		}
	
	    glDrawPixels( width, height, GL_RGB, GL_FLOAT
					, app.colorbuffer().data());
	
	    win.update();
	}
	
	thr.join();
	
	return 0;
}
