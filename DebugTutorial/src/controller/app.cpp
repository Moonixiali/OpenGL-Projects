#include "app.h"

App::App() {
    set_up_glfw();
}

App::~App() {
    for (unsigned int& shader : shaders) {
    	glDeleteProgram(shader);
	}

	delete animationSystem;
    delete motionSystem;
    delete cameraSystem;
    delete renderSystem;
    
    glfwTerminate();
}

void App::run() {

	speed = 10.0f;
	tFromFrame = 0.0f;
	fpsLimit = 120.0f;
	lTime = 0;

    while (!glfwWindowShouldClose(window)) {

		cTime = glfwGetTime();
		elapTime += cTime - lTime;
		lTime = cTime;
		if (elapTime >= 1.0f/fpsLimit) {

			std::stringstream title;
			title << "Running at " << (1.0f/elapTime) << " fps.";
			glfwSetWindowTitle(window, title.str().c_str());
			elapTime = 0.0f;
			//std::cout << "generating frame, elap is: " << elapTime << std::endl;
			motionSystem->update(tFromFrame);
			bool should_close = cameraSystem->update(tFromFrame, speed);
			if (should_close) {
				break;
			}
			animationSystem->update(tFromFrame * 1000.0f);
	
			renderSystem->update();
			
			time_since_frame();
		}
	}
}

void App::set_up_glfw() {

    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	
	window = glfwCreateWindow(1280, 720, "evil geometry", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
	}

}

void App::set_up_opengl() {

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
	//Set the rendering region to the actual screen size
	int w,h;
	glfwGetFramebufferSize(window, &w, &h);
	//(left, top, width, height)
	glViewport(0,0,w,h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	shaders.push_back(make_shader(
		"../src/shaders/vertex_sky.txt", 
		"../src/shaders/fragment_sky.txt"));
	shaders.push_back(make_shader(
		"../src/shaders/geometry_vertex.txt", 
		"../src/shaders/fragment.txt"));
	shaders.push_back(make_shader(
		"../src/shaders/vertex.txt", 
		"../src/shaders/fragment.txt"));
    
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glm::mat4 projection = glm::perspective(
		45.0f, float(windowWidth) / float(windowHeight), 0.1f, 1000.0f);
	for (size_t i = 1; i < 3; ++i) {
		unsigned int shader = shaders[i];
    	glUseProgram(shader);
		unsigned int projLocation = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	}
}

void App::make_systems() {
	animationSystem = new AnimationSystem(animationComponents);
    motionSystem = new MotionSystem(transformComponents, physicsComponents);
    cameraSystem = new CameraSystem(shaders, window, 
		transformComponents, cameraComponents);
    renderSystem = new RenderSystem(shaders, window, transformComponents, 
		renderComponents, animationComponents);
}

void App::time_since_frame() {
	fCurrentTime = glfwGetTime();
	tFromFrame = fCurrentTime - fLastTime;
	fLastTime = fCurrentTime;
}
