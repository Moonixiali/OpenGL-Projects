#pragma once
#include "../config.h"

struct RenderComponent {
    ObjectType objectType;
    AnimationType animationType;
    unsigned int material, VAO, vertexCount;
};