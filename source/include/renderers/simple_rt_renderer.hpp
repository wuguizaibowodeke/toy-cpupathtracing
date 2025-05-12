#pragma once

#include "baserenderer.hpp"

class SimpleRTRenderer : public BaseRenderer
{
public:
    explicit SimpleRTRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};