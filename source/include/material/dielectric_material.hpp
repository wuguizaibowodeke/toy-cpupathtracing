#pragma once

#include "material/material.hpp"

class DielectricMaterial : public Material
{
public:
    DielectricMaterial(const glm::vec3 &albedo, float ior);

    DielectricMaterial(const glm::vec3 &albedo_r,
                       const glm::vec3 &albedo_t,
                       float ior);

    virtual glm::vec3 sampleBRDF(const glm::vec3 &view_direction,
                                 glm::vec3 &beta,
                                 const RandomNumberGenerator &rng) const override;

    virtual glm::vec3 sampleBSDF(const glm::vec3 &hit_point,
                                 const glm::vec3 &view_direction,
                                 glm::vec3 &beta,
                                 const RandomNumberGenerator &rng) const override;

private:
    // 折射率
    float m_ior{1.0f};
    // 透射反照率
    glm::vec3 m_albedo_r{1.0f, 1.0f, 1.0f};
    // 折射透射率
    glm::vec3 m_albedo_t{1.0f, 1.0f, 1.0f};
};