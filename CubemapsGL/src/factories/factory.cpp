#include "factory.h"

Factory::Factory(
    ComponentSet<PhysicsComponent>& physicsComponents,
    ComponentSet<RenderComponent>& renderComponents,
    ComponentSet<TransformComponent>& transformComponents,
    ComponentSet<AnimationComponent>& animationComponents,
	ComponentSet<CameraComponent>& cameraComponents):
physicsComponents(physicsComponents),
renderComponents(renderComponents),
transformComponents(transformComponents),
animationComponents(animationComponents),
cameraComponents(cameraComponents) {
}

void Factory::make_camera(glm::vec3 position, glm::vec3 eulers) {

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;

    transformComponents.insert(entities_made, transform);

	CameraComponent camera;
    cameraComponents.insert(entities_made++, camera);
}

void Factory::make_sphere(glm::vec3 position, glm::vec3 eulers, 
    glm::vec3 eulerVelocity) {

	TransformComponent transform;
	transform.position = position;
	transform.eulers = eulers;
	transformComponents.insert(entities_made, transform);
	
	PhysicsComponent physics;
	physics.velocity = {0.0f, 0.0f, 0.0f};
	physics.eulerVelocity = eulerVelocity;
	physicsComponents.insert(entities_made, physics);
	
	RenderComponent render;
	render.objectType = ObjectType::eSphere;
	render.animationType = AnimationType::eNone;
	renderComponents.insert(entities_made++, render);
}

void Factory::make_revy(glm::vec3 position, glm::vec3 eulers) {

	TransformComponent transform;
	transform.position = position;
	transform.eulers = eulers;
	transformComponents.insert(entities_made, transform);

	AnimationComponent animation;
	animation.frame = 0;
	animation.speed = 0.4f;
	animation.frameCount = 19;
	animationComponents.insert(entities_made, animation);
	
	RenderComponent render;
	render.objectType = ObjectType::eRevy;
	render.animationType = AnimationType::eRun;
	renderComponents.insert(entities_made++, render);
}

void Factory::make_rat(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity) {
	
	TransformComponent transform;
	transform.position = position;
	transform.eulers = eulers;
	transformComponents.insert(entities_made, transform);
	
	PhysicsComponent physics;
	physics.velocity = {0.0f, 0.0f, 0.0f};
	physics.eulerVelocity = eulerVelocity;
	physicsComponents.insert(entities_made, physics);
	
	RenderComponent render;
	render.objectType = ObjectType::eRat;
	render.animationType = AnimationType::eNone;
	renderComponents.insert(entities_made++, render);
}