#pragma once
#include "../config.h"

#include "../components/animation_component.h"
#include "../components/render_component.h"
#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"
#include "../components/component_set.h"

#include "../systems/animation_system.h"
#include "../systems/camera_system.h"
#include "../systems/motion_system.h"
#include "../systems/render_system.h"

#include "../view/shader.h"
#include "../factories/factory.h"

class App {
public:
    App();
    ~App();
    void run(Factory* factory);
    void set_up_opengl();
    void make_systems();

    //Components
    ComponentSet<AnimationComponent> animationComponents;
    ComponentSet<TransformComponent> transformComponents;
    ComponentSet<PhysicsComponent> physicsComponents;
    ComponentSet<CameraComponent> cameraComponents;
    ComponentSet<RenderComponent> renderComponents;
    
private:
    void set_up_glfw();
    void set_up_imgui();
    void time_since_frame();

    GLFWwindow* window;

    std::vector<unsigned int> shaders;

    //Systems
    AnimationSystem* animationSystem;
    MotionSystem* motionSystem;
    CameraSystem* cameraSystem;
    RenderSystem* renderSystem;

    //Timing
    double tFromFrame, fCurrentTime, fLastTime;

    float fpsLimit, fpsCurrent;
    float cTime, lTime, elapTime;

    //Camera
    float speed;
};