#include "renderers/pathtracing_renderer.hpp"
#include "util/frame.hpp"
#include "sampler/spherical.hpp"

PathTracingRenderer::PathTracingRenderer(Camera &camera, Scene &scene)
    : BaseRenderer(camera, scene)
{
}

glm::vec3 PathTracingRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
    auto ray = m_camera.generateRay(pixel_coord, {m_rng.uniform(), m_rng.uniform()});
    glm::vec3 beta = glm::vec3(1.0f);
    glm::vec3 L = glm::vec3(0.0f);

    const float q = 0.9;
    while (true)
    {
        auto hit_info = m_scene.intersect(ray);
        if (hit_info.has_value())
        {
            if (m_rng.uniform() > q)
            {
                break;
            }

            auto material = hit_info->material;
            L += beta * material->getEmissive();
            beta *= material->getAlbedo() / q;

            glm::vec3 light_direction;
            Frame frame(hit_info->normal);
            if (material->getEnableSpecular())
            {
                glm::vec3 view_direction = frame.localFromWorld(-ray.direction);
                light_direction = {-view_direction.x, view_direction.y, -view_direction.z};
            }
            else
            {
                light_direction = CosineSampleHemisphere({m_rng.uniform(), m_rng.uniform()});
            }

            ray.origin = hit_info->hit_point;
            ray.direction = frame.worldFromLocal(light_direction);
        }
        else
        {
            break;
        }
    }

    return L;
}