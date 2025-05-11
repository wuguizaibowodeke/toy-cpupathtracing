#include "thread/thread_pool.hpp"
#include "camera/film.hpp"
#include "camera/camera.hpp"
#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
#include "model/model.hpp"
#include "model/scene.hpp"
#include "model/material.hpp"
#include "util/frame.hpp"
#include "util/rgb.hpp"
#include "util/algorithm.hpp"

#include <iostream>
#include <random>

int main()
{
    ThreadPool thread_pool{};

    Film film{1920, 1080};
    Camera camera{film, {0, 0, 1}, {0, 0, 0}, 90};

    Sphere sphere{
        {0, 0, 0},
        0.5f};
    glm::vec3 light_pos{1, 1, 1};

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)
                            {
        auto ray = camera.generateRay({ x, y });
        auto result = sphere.intersect(ray);
        if (result.has_value()) 
        {
            auto hit_point = result->hit_point;
            auto normal = glm::normalize(hit_point - sphere.getCenter());
            auto L = glm::normalize(light_pos - hit_point);
            float cosine = glm::max(0.f, glm::dot(normal, L));
            film.addSample(x, y, glm::vec3{ cosine, cosine, cosine });
        } });

    thread_pool.wait();
    film.save("sample01.ppm");
    return 0;
}