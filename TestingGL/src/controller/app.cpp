#include "app.h"

/* this is called by main at the very start of the program.
it calls the methods to setup glfw and imgui */
App::App() {
    set_up_glfw();
	set_up_imgui();
}

/* when the class goes out of scope (app is closed)
this will run and delete the systems, freeing the memory
that they took up */
App::~App() {
    for (unsigned int& shader : shaders) {
    	glDeleteProgram(shader);
	}

	delete animationSystem;
    delete motionSystem;
    delete cameraSystem;
    delete renderSystem;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    
    glfwTerminate();
}

void App::run(Factory* factory) {
	/* this method is the main loop of the app
	the variables below get set only once */
	speed = 10.0f;
	cameraSystem->change_speed(speed);

	tFromFrame = 0.0f;
	lTime = 0;

	fpsLimit = 100000.0f;

	/* this is the section that runs every frame
	this checks if the window should close, and if 
	not, then it runs the loop */
    while (!glfwWindowShouldClose(window)) {
		//these variables are for calculating fps
		cTime = glfwGetTime();
		elapTime += cTime - lTime;
		lTime = cTime;
		//prevents generating more frames than the fps limit
		if (elapTime >= 1.0f/fpsLimit) {
			fpsCurrent = 1.0f/elapTime;
			elapTime = 0.0f;

			/* setting the title of the window
			honestly shouldnt be in this main loop, but it
			wont affect performance */
			std::stringstream title;
			title << "the window";
			glfwSetWindowTitle(window, title.str().c_str());
			
			/* updating the different systems of the app
			we check if theres a camera, and if not, then we
			don't run the animation and render systems */
			motionSystem->update(tFromFrame);
			bool should_close = cameraSystem->update(tFromFrame);
			if (should_close) {
				break;
			}
			animationSystem->update(tFromFrame * 1000.0f);
			
			/* rendersystem update function renders physical things
			in the world, like objects and the sky, while the 
			build_ui function renders ui */
			renderSystem->update();
			renderSystem->build_ui(fpsCurrent, cameraSystem, factory);

			/* we push what we rendered to the window, and finally
			calculate the time difference between the start and end
			of the frame generating to get our final FPS */
			glfwSwapBuffers(window);
			time_since_frame();
		}
	}
}

/* function that sets up GLFW (openGL FrameWork) 
GLFW simplifies a lot of boilerplate code (code that
repeats across projects), speeding up development */
void App::set_up_glfw() {

    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	
	window = glfwCreateWindow(1280, 720, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
	}

}

/* function that sets up IMGUI (IMmediate Graphical User Interface)
IMGUI is an open source library used for creating UI
in C++ projects, and is very simple to use */
void App::set_up_imgui() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
}

/* function that sets up OpenGL
things such as the default window color, different OpenGL
features like depth buffers and backface culling get set here */
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

/* function that sets up systems
we create references to all of the systems in our app
so that we can call functions inside of them when needed */
void App::make_systems() {
	animationSystem = new AnimationSystem(animationComponents);
    motionSystem = new MotionSystem(transformComponents, physicsComponents);
    cameraSystem = new CameraSystem(shaders, window, 
		transformComponents, cameraComponents);
    renderSystem = new RenderSystem(shaders, window, transformComponents, 
		renderComponents, animationComponents);
}

/* function used for calculating FPS */
void App::time_since_frame() {
	fCurrentTime = glfwGetTime();
	tFromFrame = fCurrentTime - fLastTime;
	fLastTime = fCurrentTime;
}
