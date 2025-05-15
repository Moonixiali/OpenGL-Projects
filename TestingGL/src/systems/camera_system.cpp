#include "camera_system.h"

CameraSystem::CameraSystem(std::vector<unsigned int>& shaders, 
    GLFWwindow* window, ComponentSet<TransformComponent>& transforms, 
    ComponentSet<CameraComponent>& cameras):
shaders(shaders),
transforms(transforms),
cameras(cameras) {
    this->window = window;

    unsigned int shader = shaders[0];
    glUseProgram(shader);
    forwardsLocation = glGetUniformLocation(shader, "forwards");
    rightLocation = glGetUniformLocation(shader, "right");
    upLocation = glGetUniformLocation(shader, "up");
    for (size_t i = 1; i < 3; ++i) {
        shader = shaders[i];
        glUseProgram(shader);
        viewLocation.push_back(glGetUniformLocation(shader, "view"));
    }

    int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
    dy = 1.00f * glm::tan(3.14159f / 8.0f);
    dx = dy * windowWidth / windowHeight;
}

double previousTime = 0;

bool CameraSystem::update(float dt) {

    CameraComponent& camera = cameras.components[0];
    uint32_t cameraID = cameras.entities[0];

    TransformComponent& transform = transforms.get_component(cameraID);
    glm::vec3& pos = transform.position;
    glm::vec3& eulers = transform.eulers;
    float theta = glm::radians(eulers.z);
    float phi = glm::radians(eulers.y);

    glm::vec3& right = camera.right;
    glm::vec3& up = camera.up;
    glm::vec3& forwards = camera.forwards;

    forwards = {
        glm::cos(theta) * glm::cos(phi),
        glm::sin(theta) * glm::cos(phi),
        glm::sin(phi)
    };
    right = glm::normalize(glm::cross(forwards, global_up));
    up = glm::normalize(glm::cross(right, forwards));

    glm::mat4 view = glm::lookAt(pos, pos + forwards, up);

    glUseProgram(shaders[0]);
    glUniform3fv(forwardsLocation, 1, glm::value_ptr(forwards));
    glUniform3fv(rightLocation, 1, glm::value_ptr(dx * right));
    glUniform3fv(upLocation, 1, glm::value_ptr(dy * up));
    
    glUseProgram(shaders[1]);
    glUniformMatrix4fv(viewLocation[0], 1, GL_FALSE, glm::value_ptr(view));
    glUseProgram(shaders[2]);
    glUniformMatrix4fv(viewLocation[1], 1, GL_FALSE, glm::value_ptr(view));

    //Keys movement
    glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dPos.x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dPos.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dPos.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dPos.y += 1.0f;
    }
    if (glm::length(dPos) != 0.0f) {
        dPos = glm::normalize(dPos);
        pos += dt * speed * dPos.x * forwards;
        pos += dt * speed * dPos.y * right;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return true;
    }

    //when right click is held, this runs every frame
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

        //find centre of the window and store as centerX and centerY
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        double centerX = windowWidth / 2.0;
        double centerY = windowHeight / 2.0;

        //disable cursor visibility and put the cursor to the centre of the screen
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, centerX, centerY);

        //update cursor position in program
        glfwPollEvents();

        //get cursor position, remember that the center is 0.0, 0.0
        glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        //move the camera rotation by however much the mouse has moved this frame
        //from the centre
        dEulers.z = -0.1f * static_cast<float>(mouse_x - centerX);
        dEulers.y = -0.1f * static_cast<float>(mouse_y - centerY);

        //prevent the camera rotation from going further than 89 degrees up and down
        eulers.y = fminf(89.0f, fmaxf(-89.0f, eulers.y + dEulers.y));

        //normalize Z rotation to 0-360 if it goes further than 360 or lower than 0
        eulers.z += dEulers.z;
        if (eulers.z > 360) {
            eulers.z -= 360;
        }
        else if (eulers.z < 0) {
            eulers.z += 360;
        }
    }
    else { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
    glfwPollEvents();

    //perspective

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
    glViewport(0, 0, windowWidth, windowHeight);

    return false;
}

void CameraSystem::change_speed(float speedNew) {
    speed = speedNew;
};
