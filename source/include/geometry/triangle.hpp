#pragma once

#include "geometry/shape.hpp"

class Triangle : public Shape
{
public:
    Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
             const glm::vec3 &normal0, const glm::vec3 &normal1, const glm::vec3 &normal2);

    Triangle(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2);

    ~Triangle() {};

    glm::vec3 getVertex(int index) const;

    glm::vec3 getNormal(int index) const;

    virtual std::optional<RayHitInfo> intersect(const Ray &ray, float t_min,
                                        float t_max) const override;

    virtual Bound getBound() const override;
    
private:
    // 顶点坐标
    glm::vec3 v0, v1, v2;
    // 法向量
    glm::vec3 normal0, normal1, normal2;
};
