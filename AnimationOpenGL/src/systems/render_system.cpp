#include "render_system.h"
#include <iostream>

RenderSystem::RenderSystem(unsigned int shader, GLFWwindow* window) {
    
    modelLocation = glGetUniformLocation(shader, "model");
    this->window = window;

    ObjectType objectType = ObjectType::eBox;
    AnimationType animationType = AnimationType::eNone;
    MeshFactory meshFactory;
    StaticMesh mesh = meshFactory.make_cube_mesh({0.25f, 0.25f, 0.25f});
    VAOs[objectType][animationType].push_back(mesh.VAO);
    VBOs[objectType][animationType].push_back(mesh.VBO);
    vertexCounts[objectType] = mesh.vertexCount;
    textures[objectType] = meshFactory.make_texture("../img/paper.jpg");
	
    objectType = ObjectType::eRat;
    animationType = AnimationType::eNone;
    glm::mat4 preTransform = glm::mat4(1.0f);
	preTransform = glm::rotate(preTransform, 
        glm::radians(0.0f), { 1.0f, 0.0f, 0.0f });
    preTransform = glm::rotate(preTransform, 
        glm::radians(0.0f), { 0.0f, 1.0f, 0.0f });
	mesh = meshFactory.make_obj_mesh("../models/rat.obj", preTransform);
	VAOs[objectType][animationType].push_back(mesh.VAO);
    VBOs[objectType][animationType].push_back(mesh.VBO);
    vertexCounts[objectType] = mesh.vertexCount;
    textures[objectType] = meshFactory.make_texture("../img/tex_rat.png");

    objectType = ObjectType::eRevy;
    animationType = AnimationType::eRun;
    preTransform = glm::mat4(1.0f);
	preTransform = glm::rotate(preTransform, 
        glm::radians(0.0f), { 1.0f, 0.0f, 0.0f });
    preTransform = glm::rotate(preTransform, 
        glm::radians(0.0f), { 0.0f, 1.0f, 0.0f });
	mesh = meshFactory.make_obj_mesh("../models/Revy_run_000000.obj", preTransform);
    textures[objectType] = meshFactory.make_texture("../img/Revy_Final.png");
    std::stringstream filepath;
    std::string built_filepath;
    for (size_t i = 0; i < 19; ++i) {
        filepath.str("");
        filepath << "../models/Revy_run_0000";
        if (i < 10) {
            filepath << "0";
        }
        filepath << i << ".obj";
        built_filepath = filepath.str();
        mesh = meshFactory.make_obj_mesh(built_filepath.c_str(), preTransform);
        VAOs[objectType][animationType].push_back(mesh.VAO);
        VBOs[objectType][animationType].push_back(mesh.VBO);
        vertexCounts[objectType] = mesh.vertexCount;
    }
}
    
void RenderSystem::update(
    std::unordered_map<unsigned int,TransformComponent> &transformComponents,
    std::unordered_map<unsigned int,RenderComponent> &renderComponents,
    std::unordered_map<unsigned int, AnimationComponent> &animationComponents) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (auto& [entity,renderable] : renderComponents) {

        TransformComponent& transform = transformComponents[entity];
        glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, transform.position);
	    model = glm::rotate(
            model, glm::radians(transform.eulers.z), 
            { 0.0f, 0.0f, 1.0f });
        model = glm::rotate(
            model, glm::radians(transform.eulers.y), 
            { 0.0f, 1.0f, 1.0f });
        model = glm::rotate(
            model, glm::radians(transform.eulers.x), 
            { 1.0f, 0.0f, 0.0f });
        glUniformMatrix4fv(
		    modelLocation, 1, GL_FALSE, 
		    glm::value_ptr(model));
        
        glBindTexture(GL_TEXTURE_2D, textures[renderable.objectType]);
        unsigned int vertexCount = vertexCounts[renderable.objectType];
        size_t frame = 0;
        if (renderable.animationType != AnimationType::eNone) {
            frame = static_cast<size_t>(animationComponents[entity].frame);
        }
        glBindVertexArray(VAOs[renderable.objectType][renderable.animationType][frame]);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
	glfwSwapBuffers(window);
}