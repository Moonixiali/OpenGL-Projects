#include "config.h"
#include "controller/app.h"

#include "components/camera_component.h"

#include "factories/factory.h"

int main() {

	App* app = new App();
	Factory* factory = new Factory(
		app->physicsComponents, app->renderComponents, 
		app->transformComponents, app->animationComponents,
		app->cameraComponents);

	factory->make_sphere(
		{0.0f, 0.0f, 4.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

	factory->make_revy({4.0f, 2.0f, 0.25f}, {0.0f, 0.0f, 270.0f});
	
	factory->make_rat({-3.0f, -3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {100.0f, 150.0f, 200.f});

	factory->make_camera(
		{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f,0.0f});

	app->set_up_opengl();
	app->make_systems();

	app->run();

	delete app;
	return 0;
}