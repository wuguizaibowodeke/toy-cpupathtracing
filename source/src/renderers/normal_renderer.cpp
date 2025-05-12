#include "renderers/normal_renderer.hpp"

NormalRenderer::NormalRenderer(Camera &camera, Scene &scene)
: BaseRenderer(camera, scene)
{
}

glm::vec3 NormalRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
    auto ray = m_camera.generateRay(pixel_coord);
    auto hit_info = m_scene.intersect(ray);
    if (hit_info.has_value()) 
    {
        return hit_info->normal * 0.5f + glm::vec3(0.5f);
    }

    return glm::vec3(0.0f);
}
