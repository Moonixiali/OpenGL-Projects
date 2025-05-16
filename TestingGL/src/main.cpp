#include "config.h"
#include "controller/app.h"

#include "components/camera_component.h"

#include "factories/factory.h"

/* Main program method
This method runs when the program is ran */
int main() {

	//Create the app and factory so we can reference them
	App* app = new App();
	Factory* factory = new Factory(
		app->physicsComponents, app->renderComponents, 
		app->transformComponents, app->animationComponents,
		app->cameraComponents);

	//create the objects in our scene
	factory->make_camera(
		{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f,0.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 4.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 6.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});

	factory->make_sphere(
		{0.0f, 0.0f, 8.25f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 20.0f});
	
	factory->make_revy(
		{4.0f, 2.0f, 0.25f}, {0.0f, 0.0f, 270.0f});
	
	factory->make_rat(
		{-3.0f, -3.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {90.0f, 180.0f, 360.f});
	
	/* the army
	uncommenting this loop will create 100 uniformally spaced rats */
	/*for (int i = -20; i < 20; i=i+4) {
		for (int ii = -15; ii < 15; ii=ii+3) {
			factory->make_rat(
				{i, ii, 10.0f}, {0.0f, 0.0f, 45.0f}, {0.0f, 0.0f, 0.f});
		}
	}*/

	//tell our app to setup opengl and create the references to the systems
	app->set_up_opengl();
	app->make_systems();

	//begins the "update" method of our app
	app->run(factory);

	//when the update method is no longer running, this code will run
	//to delete the app from memory
	delete app;
	return 0;
}