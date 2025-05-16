#pragma once
#include "../config.h"

#include "../components/transform_component.h"
#include "../components/render_component.h"
#include "../components/animation_component.h"
#include "../components/component_set.h"
#include "../factories/factory.h"
#include "../systems/camera_system.h"

// a header file! :D
// see the "#include"s above? we can figure out what
// other scripts contain by including their header files.
// these header files do something called forward declaration,
// meaning we say what a specific script should contain before
// that content even makes it to memory
//
// this means that if we want to call a public method present in
// a script called "script1.cpp", we include the "script1.h" file
// which contains the method we're trying to call just like how
// we do so below
class RenderSystem {
public:

    RenderSystem(std::vector<unsigned int>& shaders, GLFWwindow* window, 
        ComponentSet<TransformComponent> &transforms,
        ComponentSet<RenderComponent> &renderables,
        ComponentSet<AnimationComponent> &animations);
    ~RenderSystem();
    
    void update();
    void build_ui(float fps, CameraSystem* camera, Factory* factory);

// the stuff below here is private, meaning it can only be accessed
// by render_system.cpp. other scripts won't be able to see the 
// methods and variables below
private:

    void build_sky();
    void build_models();
    void build_geometry();
    void build_imgui();

    //ui variables
    float myColor[4] = {255, 255, 255, 255};
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 velocity = {0, 0, 0};
    bool active;
    float speedNew = 10.0f;
    int objectType = 0;
    // eSphere 0
    // eRevy 1
    // eRat 2
    int objectDeleteId = 0;

    int objectsToCreate;
    glm::vec3 boundsPos = {0, 0, 0};
    glm::vec3 boundsRot = {0, 0, 0};
    glm::vec3 boundsVel = {0, 0, 0};

    //rendering variables
    std::vector<unsigned int>& shaders;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> EBOs;
    std::unordered_map<ObjectType, unsigned int> elementCounts;
    std::unordered_map<ObjectType, 
        std::unordered_map<AnimationType, unsigned int>> offsets;
    std::vector<unsigned int> textures;
    unsigned int skyTexture;
    unsigned int modelLocation;
    GLFWwindow* window;

    ComponentSet<TransformComponent> &transforms;
    ComponentSet<RenderComponent> &renderables;
    ComponentSet<AnimationComponent> &animations;
};