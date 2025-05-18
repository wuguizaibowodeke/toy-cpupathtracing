#pragma once

#include <glm/glm.hpp>


class Material
{
public:
    Material();

    ~Material() = default;

    Material(const glm::vec3 &albedo ,
             const glm::vec3 &emissive,
             bool is_specular);

    glm::vec3 getAlbedo() const { return m_albedo; }

    glm::vec3 getEmissive() const { return m_emissive; }

    bool getEnableSpecular() const { return m_is_specular; }

private:
    // 反照率，决定了物体的颜色
    glm::vec3 m_albedo{1.0f, 1.0f, 1.0f};

    // 自发光
    glm::vec3 m_emissive{0.0f, 0.0f, 0.0f};

    // 反射类型：镜面反射还是漫反射
    bool m_is_specular{false};
};
/*
struct Material {
    glm::vec3 albedo = { 1, 1, 1 };
    bool is_specular = false;
    glm::vec3 emissive = { 0, 0, 0 };
};*/