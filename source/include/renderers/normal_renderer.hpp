#pragma once

#include "baserenderer.hpp"

class NormalRenderer : public BaseRenderer
{
public:
    explicit NormalRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};