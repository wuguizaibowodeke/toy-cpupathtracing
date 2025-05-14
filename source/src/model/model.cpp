#include "model/model.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "util/profile.hpp"
#include <rapidobj/rapidobj.hpp>

Model::Model(const std::vector<Triangle> &triangles)
    : m_triangles(triangles)
{
    calculateBound();
}

Model::Model(const std::filesystem::path &filename)
{
    PROFILE("Load model" + filename.string())

    auto result = rapidobj::ParseFile(filename, rapidobj::MaterialLibrary::Ignore());

    for (const auto &shape : result.shapes)
    {
        size_t index_offset = 0;
        for (size_t num_face_vectex : shape.mesh.num_face_vertices)
        {
            if (num_face_vectex == 3)
            {
                auto index = shape.mesh.indices[index_offset];
                glm::vec3 pos0{
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]};
                index = shape.mesh.indices[index_offset + 1];
                glm::vec3 pos1{
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]};
                index = shape.mesh.indices[index_offset + 2];
                glm::vec3 pos2{
                    result.attributes.positions[index.position_index * 3 + 0],
                    result.attributes.positions[index.position_index * 3 + 1],
                    result.attributes.positions[index.position_index * 3 + 2]};

                if (index.normal_index >= 0)
                {
                    index = shape.mesh.indices[index_offset];
                    glm::vec3 normal0{
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]};
                    index = shape.mesh.indices[index_offset + 1];
                    glm::vec3 normal1{
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]};
                    index = shape.mesh.indices[index_offset + 2];
                    glm::vec3 normal2{
                        result.attributes.normals[index.normal_index * 3 + 0],
                        result.attributes.normals[index.normal_index * 3 + 1],
                        result.attributes.normals[index.normal_index * 3 + 2]};
                    m_triangles.push_back(Triangle{
                        pos0, pos1, pos2, normal0, normal1, normal2});
                }
                else
                {
                    m_triangles.push_back(Triangle{
                        pos0, pos1, pos2});
                }
            }
            index_offset += num_face_vectex;
        }
    }

    calculateBound();
}

std::optional<RayHitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const
{
    if (!m_bound.isIntersect(ray, t_min, t_max))
    {
        return std::nullopt;
    }

    std::optional<RayHitInfo> closest_hitinfo{};

    for (auto &triangle : m_triangles)
    {
        auto hitinfo = triangle.intersect(ray, t_min, t_max);
        if (hitinfo.has_value())
        {
            t_max = hitinfo->t;
            closest_hitinfo = hitinfo;
        }
    }
    return closest_hitinfo;
}

void Model::calculateBound()
{
    for (const auto &triangle : m_triangles)
    {
        m_bound.expand(triangle.getVertex(0));
        m_bound.expand(triangle.getVertex(1));
        m_bound.expand(triangle.getVertex(2));
    }
}
