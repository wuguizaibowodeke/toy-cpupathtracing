#include "material/dielectric_material.hpp"

float FresnelSchlick(float etai_div_etat, float cos_theta_t, float& cos_theta_i)
{
    float sin2_theta_t = 1.0f - cos_theta_t * cos_theta_t;
    float sin2_theta_i = sin2_theta_t / (etai_div_etat * etai_div_etat);

    if (sin2_theta_i >= 1.0f)
    {
        return 1.0f;
    }

    cos_theta_i = glm::sqrt(1.0f - sin2_theta_i);
    float r_parallel = (cos_theta_i - etai_div_etat * cos_theta_t) /
                       (cos_theta_i + etai_div_etat * cos_theta_t);
    float r_perpendicular = (etai_div_etat * cos_theta_i - cos_theta_t) /
                            (etai_div_etat * cos_theta_i + cos_theta_t);
    return (r_parallel * r_parallel + r_perpendicular * r_perpendicular) / 2.0f;
}

DielectricMaterial::DielectricMaterial(const glm::vec3 &albedo, float ior)
    : m_albedo_r(albedo), m_albedo_t(albedo), m_ior(ior)
{
    m_enable_refraction = true;
}

DielectricMaterial::DielectricMaterial(const glm::vec3 &albedo_r, const glm::vec3 &albedo_t, float ior)
    : m_albedo_r(albedo_r), m_albedo_t(albedo_t), m_ior(ior)
{
    m_enable_refraction = true;
}

glm::vec3 DielectricMaterial::sampleBRDF(const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    beta *= m_albedo_r;
    return glm::vec3(-view_direction.x, view_direction.y, -view_direction.z); // 反射方向
}

glm::vec3 DielectricMaterial::sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    float etai_div_etat = m_ior;
    glm::vec3 normal = {0, 1, 0};
    float cos_theta_t = view_direction.y;
    if (cos_theta_t < 0)
    {
        etai_div_etat = 1.f / m_ior;
        normal = -normal;
        cos_theta_t = -cos_theta_t;
    }

    float cos_theta_i;
    float Fr = FresnelSchlick(etai_div_etat, cos_theta_t, cos_theta_i);

    if (rng.uniform() <= Fr) // 反射
    {
        return sampleBRDF(view_direction, beta, rng);
    }
    else // 透射
    {
        beta *= m_albedo_t;
        return glm::vec3{(-view_direction / etai_div_etat) +
                         (cos_theta_t / etai_div_etat - cos_theta_i) * normal};
    }

    return glm::vec3();
}
