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
    m_scene.intersect(ray);
    return RGB::GenerateHeapmaoColor(ray.bounds_test_count / 200.f);
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
    m_scene.intersect(ray);
    return RGB::GenerateHeapmaoColor(ray.triangle_test_count / 20.f);
#else
    return glm::vec3(0.0f);
#endif
}
