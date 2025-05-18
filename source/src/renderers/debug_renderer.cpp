#include "renderers/debug_renderer.hpp"
#include "util/rgb.hpp"
#include "log.hpp"

BoundsTestCountRenderer::BoundsTestCountRenderer(Camera &camera, Scene &scene)
    : BaseRenderer(camera, scene)
{
}

glm::vec3 BoundsTestCountRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
#ifdef WITH_DEBUG_INFO
    auto ray = m_camera.generateRay(pixel_coord);
    auto hit_info = m_scene.intersect(ray);
    if (hit_info.has_value())
    {
        return RGB::GenerateHeapmaoColor(hit_info->bounds_test_count / 200.f);
    }

    return glm::vec3(0.0f);
#else
    return glm::vec3(0.0f);
#endif
}

TriangleTestCountRenderer::TriangleTestCountRenderer(Camera &camera, Scene &scene)
    : BaseRenderer(camera, scene)
{
}

glm::vec3 TriangleTestCountRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
#ifdef WITH_DEBUG_INFO
    auto ray = m_camera.generateRay(pixel_coord);
    auto hit_info = m_scene.intersect(ray);
    if (hit_info.has_value())
    {
        return RGB::GenerateHeapmaoColor(hit_info->triangle_test_count / 20.f);
    }

    return glm::vec3(0.0f);
#else
    return glm::vec3(0.0f);
#endif
}

BoundsDepthRenderer::BoundsDepthRenderer(Camera &camera, Scene &scene)
    : BaseRenderer(camera, scene)
{
}

glm::vec3 BoundsDepthRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
#ifdef WITH_DEBUG_INFO
    auto ray = m_camera.generateRay(pixel_coord);
    auto hit_info = m_scene.intersect(ray);
    if (hit_info.has_value())
    {
        return RGB::GenerateHeapmaoColor(hit_info->bounds_depth / 32.f);
    }

    return glm::vec3(0.0f);
#else
    return glm::vec3(0.0f);
#endif
}
