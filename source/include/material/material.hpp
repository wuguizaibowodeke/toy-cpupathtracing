#pragma once

#include <glm/glm.hpp>
#include "util/algorithm.hpp"
#include <memory>

class Material
{
public:
    /**
     * @brief 根据材质接受一个入射方向和一个随机数生成器，返回一个出射方向
     */
    virtual glm::vec3 sampleBRDF(const glm::vec3 &view_direction,
                                 glm::vec3 &beta,
                                 const RandomNumberGenerator &rng) const = 0;

    virtual glm::vec3 sampleBSDF(const glm::vec3 &hit_point,
                                 const glm::vec3 &view_direction,
                                 glm::vec3 &beta,
                                 const RandomNumberGenerator &rng) const = 0;

    virtual void setEmissive(const glm::vec3 &emissive) { m_emissive = emissive; }

    virtual glm::vec3 getEmissive() const { return m_emissive; }

    virtual void setRefraction(const bool enable) { m_enable_refraction = enable; }

    virtual bool getEnableRefraction() const { return m_enable_refraction; }

protected:
    glm::vec3 m_emissive{0.0f, 0.0f, 0.0f}; // 自发光

    // 是否开启折射
    bool m_enable_refraction{false};
};

using MaterialPtr = std::shared_ptr<Material>;