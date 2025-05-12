#pragma once

#include "camera/camera.hpp"
#include "model/scene.hpp"
#include "util/algorithm.hpp"

#include <filesystem>

/*
#define DEFINE_RENDERER(Name) \
    class Name##Renderer : public BaseRenderer \
    { \
    public: \
        explicit Name##Renderer(Camera &camera, Scene &scene) : BaseRenderer(camera, scene) {} \
        glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) override; \
    };\*/

class BaseRenderer
{
public:
    explicit BaseRenderer(Camera &camera, Scene &scene);

    void render(size_t spp, const std::filesystem::path& path);

    virtual glm::vec3 renderPixel(const glm::ivec2 &pixel_coord) = 0;

protected:
    Camera &m_camera;
    Scene &m_scene;
    RandomNumberGenerator m_rng{};
};