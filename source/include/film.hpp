#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <filesystem>

struct Pixel
{
    glm::vec3 color{0, 0, 0};
    size_t sample_count = 0;
};
class Film
{
public:
    Film(size_t width, size_t height);

    ~Film();

    void save(const std::filesystem::path &filename);

    size_t getWidth() const;

    size_t getHeight() const;

    Pixel getPixel(size_t x, size_t y) const;

    void addSample(size_t x, size_t y, const glm::vec3 &color);

private:
    size_t m_width, m_height;

    std::vector<Pixel> m_pixels;
};


