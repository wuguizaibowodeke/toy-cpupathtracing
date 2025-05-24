#include "model/scene.hpp"
#include <glm/ext/matrix_transform.hpp>

Scene::~Scene()
{
}

void Scene::addInstance(ShapePtr shape, MaterialPtr material, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotate)
{
    glm::mat4 world_from_object =
        glm::translate(glm::mat4(1.f), pos) *
        glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), {0, 0, 1}) *
        glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), {0, 1, 0}) *
        glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), {1, 0, 0}) *
        glm::scale(glm::mat4(1.f), scale);
    m_instances.push_back(Instance{shape, material, world_from_object, glm::inverse(world_from_object)});
}

std::optional<RayHitInfo> Scene::intersect(const Ray &ray, float t_min, float t_max) const
{
    return m_bvh.intersect(ray, t_min, t_max); 
}

void Scene::buildBVH()
{
    m_bvh.build(std::move(m_instances));
}
