#pragma once

#include "baserenderer.hpp"

class PathTracingRenderer : public BaseRenderer
{
public:
    explicit PathTracingRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};