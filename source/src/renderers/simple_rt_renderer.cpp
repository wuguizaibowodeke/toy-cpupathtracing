#include "renderers/simple_rt_renderer.hpp"
#include "util/frame.hpp"

SimpleRTRenderer::SimpleRTRenderer(Camera &camera, Scene &scene)
    : BaseRenderer(camera, scene)
{
}

glm::vec3 SimpleRTRenderer::renderPixel(const glm::ivec2 &pixel_coord)
{
    auto ray = m_camera.generateRay(pixel_coord,
                                  {std::abs(m_rng.uniform()), std::abs(m_rng.uniform())});
    glm::vec3 beta{1, 1, 1};
    glm::vec3 color{0, 0, 0};
    while (true)
    {
        auto hit_info = m_scene.intersect(ray);
        if (hit_info.has_value())
        {
            color += beta * hit_info->material->getEmissive();
            beta *= hit_info->material->getAlbedo();

            ray.origin = hit_info->hit_point;
            Frame frame(hit_info->normal);
            glm::vec3 light_direction;
            if (hit_info->material->getEnableSpecular())
            {
                glm::vec3 view_direction = frame.localFromWorld(-ray.direction);
                light_direction = {-view_direction.x, view_direction.y, view_direction.z};
            }
            else
            {
                do
                {
                    light_direction = {m_rng.uniform(), m_rng.uniform(), m_rng.uniform()};
                } while (glm::length(light_direction) > 1.0f);
                if (light_direction.y < 0)
                {
                    light_direction.y = -light_direction.y;
                }
            }
            ray.direction = frame.worldFromLocal(light_direction);
        }
        else
        {
            break;
        }
    }
    
    return color;
}