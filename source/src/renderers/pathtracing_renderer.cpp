#include "renderers/pathtracing_renderer.hpp"
#include "util/frame.hpp"

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
            auto material = hit_info->material;
            if (material == nullptr)
            {
                break;
            }
            L += beta * material->getEmissive();
            if (m_rng.uniform() > q)
            {
                break;
            }
            beta /= q; 

            Frame frame(hit_info->normal);
            glm::vec3 light_direction;
            glm::vec3 view_direction = frame.localFromWorld(-ray.direction);
            if(material->getEnableRefraction())
            {
                light_direction = material->sampleBSDF(hit_info->hit_point, view_direction, beta, m_rng);
            }
            else
            {
                light_direction = material->sampleBRDF(view_direction, beta, m_rng);
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