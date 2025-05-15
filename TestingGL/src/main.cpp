#include "config.h"
#include "controller/app.h"

#include "components/camera_component.h"

#include "factories/factory.h"

/* Main program function*/
int main() {

	App* app = new App();
	Factory* factory = new Factory(
		app->physicsComponents, app->renderComponents, 
		app->transformComponents, app->animationComponents,
		app->cameraComponents);

	factory->make_camera(
		{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f,0.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 4.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 6.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 8.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});
	
	factory->destroy_camera(0);

	factory->make_revy(
		{4.0f, 2.0f, 0.25f}, {0.0f, 0.0f, 270.0f});
	
	factory->make_rat(
		{-3.0f, -3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {90.0f, 180.0f, 360.f});

	factory->make_camera(
		{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f,0.0f});

	app->set_up_opengl();
	app->make_systems();

	app->run(factory);

	delete app;
	return 0;
}