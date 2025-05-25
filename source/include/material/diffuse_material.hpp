#pragma once

#include "material/material.hpp"

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const glm::vec3 &albedo);

    virtual glm::vec3 sampleBRDF(const glm::vec3 &view_direction,
                     glm::vec3 &beta,
                     const RandomNumberGenerator &rng) const override;

private:
    glm::vec3 m_albedo{1.0f, 1.0f, 1.0f};
};