#pragma once

#include <glm/glm.hpp>
#include "camera/ray.hpp"

class Bound
{
public:
    Bound(); 

    Bound(const glm::vec3 &leftBottom, const glm::vec3 &rightTop);


    const glm::vec3 &getLeftBottom() const;

    const glm::vec3 &getRightTop() const;

    void setLeftBottom(const glm::vec3 &leftBottom);

    void setRightTop(const glm::vec3 &rightTop);

    void setBounds(const glm::vec3 &leftBottom, const glm::vec3 &rightTop);
    
    void expand(const glm::vec3 &point);

    void expand(const Bound &other);
    
    bool isIntersect(const Bound &other) const;

    bool isIntersect(const glm::vec3 &point) const;

    bool isIntersect(const Ray &ray, float t_min, float t_max) const;

    bool isIntersect(const Ray &ray, glm::vec3 inv_dir, float t_min, float t_max) const;

    glm::vec3 diagonal() const;

    float area() const;

private:
    glm::vec3 m_leftBottom;
    glm::vec3 m_rightTop;
};