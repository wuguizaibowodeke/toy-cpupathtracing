#pragma once

#include "renderers/baserenderer.hpp"

class BoundsTestCountRenderer : public BaseRenderer
{
public:
    explicit BoundsTestCountRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};

class TriangleTestCountRenderer : public BaseRenderer
{
public:
    explicit TriangleTestCountRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};

class BoundsDepthRenderer : public BaseRenderer
{
public:
    explicit BoundsDepthRenderer(Camera &camera, Scene &scene);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override;
};