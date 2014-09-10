#include <thread>
#include <renderer.hpp>
#include <fensterchen.hpp>

int main(int argc, char* argv[])
{

	// CREATE VECTORS AND POINTS ----------------------------------------
	glm::ivec3 origin(0, 0, 0);	
	glm::ivec3 x(1, 0, 0);
	glm::ivec3 y(0, 1, 0);
	glm::ivec3 z(0, 0, 1);

	glm::ivec3 p1(3, 3, -6);
	glm::ivec3 p2(-1, -0.5, -5);
	glm::ivec3 p3(2, 0, -3);

	// CAMERA -----------------------------------------------------------
	glm::ivec3 campos(0, 0, 5);
	glm::ivec3 lookat(0, 0, -1);

	glm::ivec3 camdir(lookat - campos); 
	glm::ivec3 camright = glm::cross(camdir, y);
	glm::ivec3 camdown = glm::cross(camdir, camright);

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
	glm::ivec3 light_position(5, 5, 10);
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

	// ???
	double accuracy 	= 0.000001;

	// WINDOW RESOLUTION
	const int width		= 800;
	const int height	= 600;
	const double aspectratio = (double) width / (double) height;

	// CREATE OUTPUT WINDOW
	//glutwindow::init(width, height, 100, 100, "RayWalker", argc, argv);
	std::cout << "Hello, I am RayWalker." << std::endl;
	
	unsigned const width = 600;
	unsigned const height = 600;
	std::string const filename = "./raywalker.ppm";
############################################################################################################################
	Renderer app(width, height, filename);
	Renderer rendi(scene_objects, sources, width, height, aspectratio, accuracy, ambientlight, scenecam);
	std::thread thr(std::bind(&Renderer::render, &rendi));
	glutwindow::instance().run();


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
