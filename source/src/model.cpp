#include "model.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Model::Model(const std::vector<Triangle> &triangles)
:m_triangles(triangles)
{
}

Model::Model(const std::filesystem::path &filename)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open model file: " << filename << std::endl;
        return;
    }

    std::string line = "";
    char trash;
    while(!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line);

        if(line.compare(0, 2, "v ") == 0) //顶点坐标
        {
            glm::vec3 position;
            iss >> trash >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }

        if(line.compare(0, 3, "vn ") == 0) //顶点法向量
        {
            glm::vec3 normal;
            iss >> trash >> trash >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }

        if(line.compare(0, 2, "f ") == 0)//三角面
        {
            glm::ivec3 idx_v, idx_vn;
            iss >> trash;
            iss >> idx_v.x >> trash >> trash >> idx_vn.x;
            iss >> idx_v.y >> trash >> trash >> idx_vn.y;
            iss >> idx_v.z >> trash >> trash >> idx_vn.z;

            m_triangles.push_back(Triangle(
                positions[idx_v.x - 1],
                positions[idx_v.y - 1],
                positions[idx_v.z - 1],
                normals[idx_vn.x - 1],
                normals[idx_vn.y - 1],
                normals[idx_vn.z - 1]));
        }
    }
}

std::optional<RayHitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const
{
    std::optional<RayHitInfo> closest_hitinfo{};

    for(auto & triangle : m_triangles)
    {
        auto hitinfo = triangle.intersect(ray, t_min, t_max);
        if(hitinfo.has_value())
        {
            t_max = hitinfo->t;
            closest_hitinfo = hitinfo;
        }
    }
    return closest_hitinfo;
}
