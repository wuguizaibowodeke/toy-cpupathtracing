#include "camera/film.hpp"
#include "util/rgb.hpp"
#include <fstream>

Film::Film(size_t width, size_t height)
    : m_width(width), m_height(height)
{
    m_pixels.resize(m_width * m_height);
}

Film::~Film()
{
}

void Film::save(const std::filesystem::path &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing");
    }

    // Write PPM header
    file << "P6\n"
         << m_width << ' ' << m_height << "\n255\n";
    for (size_t y = 0; y < m_height; ++y)
    {
        for (size_t x = 0; x < m_width; ++x)
        {
            auto pixel = getPixel(x, y);
            RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
            file << static_cast<uint8_t>(rgb.r)
                 << static_cast<uint8_t>(rgb.g)
                 << static_cast<uint8_t>(rgb.b);
        }
    }
}

size_t Film::getWidth() const
{
    return m_width;
}

size_t Film::getHeight() const
{
    return m_height;
}

Pixel Film::getPixel(size_t x, size_t y) const
{
    return m_pixels[y * m_width + x];
}

void Film::addSample(size_t x, size_t y, const glm::vec3 &color)
{
    m_pixels[y * m_width + x].color += color;
    m_pixels[y * m_width + x].sample_count++;
}
