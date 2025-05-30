#pragma once
#include "../config.h"
#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/animation_component.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"
#include "../components/component_set.h"

class Factory {

public:
    Factory(
        ComponentSet<PhysicsComponent>& physicsComponents,
        ComponentSet<RenderComponent>& renderComponents,
        ComponentSet<TransformComponent>& transformComponents,
        ComponentSet<AnimationComponent>& animationComponents,
        ComponentSet<CameraComponent>& cameraComponents);
    
    ~Factory();

    void make_camera(glm::vec3 position, glm::vec3 eulers);

    void make_sphere(glm::vec3 position, glm::vec3 eulers, 
        glm::vec3 eulerVelocity);

    void make_revy(glm::vec3 position, glm::vec3 eulers);
    
    void make_rat(glm::vec3 position, glm::vec3 eulers,
        glm::vec3 eulerVelocity);

private:

    unsigned int entities_made = 0;

    ComponentSet<PhysicsComponent>& physicsComponents;
    ComponentSet<RenderComponent>& renderComponents;
    ComponentSet<TransformComponent>& transformComponents;
    ComponentSet<AnimationComponent>& animationComponents;
    ComponentSet<CameraComponent>& cameraComponents;

};