#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <vector>
#include <unordered_map>

#include <fstream>
#include <sstream>
#include <string>

std::vector<std::string> split(std::string line, std::string delimiter);

enum class ObjectType {
    eSphere,
    eGeometry,
    eRevy,
    eRat
};

enum class AnimationType {
    eNone,
    eRun,
};