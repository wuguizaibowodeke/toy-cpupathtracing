#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <filesystem>

class Film
{
public:
    Film(size_t width, size_t height);

    ~Film();

    void save(const std::filesystem::path &filename);

    size_t getWidth() const;

    size_t getHeight() const;

    glm::vec3 getPixel(size_t x, size_t y) const;

    void setPixel(size_t x, size_t y, const glm::vec3 &color);

private:
    size_t m_width, m_height;

    std::vector<glm::vec3> m_pixels;
};


