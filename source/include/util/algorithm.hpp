#pragma once

#include <random>
/**
 * @brief 生成随机数
 */
class RandomNumberGenerator
{
public:
    RandomNumberGenerator(size_t seed)
    {
        setSeed(seed);
    }

    RandomNumberGenerator() : RandomNumberGenerator(0)
    {
    }

    void setSeed(size_t seed)
    {
        m_generator.seed(seed);
    }

    float uniform()
    {
        return m_distribution(m_generator);
    }

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution{0, 1};
};