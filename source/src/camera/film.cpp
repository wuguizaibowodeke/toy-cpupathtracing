#include "camera/film.hpp"
#include "util/rgb.hpp"
#include <fstream>
#include "util/profile.hpp"
#include "thread/thread_pool.hpp"

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
    PROFILE("Save film to " + filename.string())
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing");
    }

    // Write PPM header
    file << "P6\n"
         << m_width << ' ' << m_height << "\n255\n";

    std::vector<uint8_t> buffer(m_width * m_height * 3);
    
    thread_pool.parallelFor(m_width, m_height, [&](size_t x, size_t y)
    {
        auto pixel = getPixel(x, y);
        RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
        auto idx = (y * m_width + x) * 3;
        buffer[idx] = static_cast<uint8_t>(rgb.r);
        buffer[idx + 1] = static_cast<uint8_t>(rgb.g);
        buffer[idx + 2] = static_cast<uint8_t>(rgb.b);
    });
    thread_pool.wait();

    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
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

void Film::clear()
{
    m_pixels.clear();
    m_pixels.resize(m_width * m_height);
}
