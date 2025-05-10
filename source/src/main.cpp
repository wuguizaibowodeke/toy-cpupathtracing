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

#include <iostream>
#include <random>

int main()
{
    ThreadPool thread_pool{};

    Film film{192 * 4, 108 * 4};
    Camera camera{film, {-3.6, 0, 0}, {0, 0, 0}, 45};
    std::atomic<int> count = 0;

    Model model("asset/models/simple_dragon.obj");
    Sphere sphere{
        {0, 0, 0},
        1.0f};
    Plane plane{
        {0, 0, 0},
        {0, 1, 0}};

    RGB red{255, 0, 0};
    RGB green{0, 255, 0};
    RGB blue{0, 0, 255};

    Material red_material{glm::vec3{1, 1, 1}, red, false};
    Material green_material{glm::vec3{1, 1, 1}, green, false};
    Material blue_material{glm::vec3{1, 1, 1}, blue, false};

    Scene scene{};
    scene.addInstance(model,
                      red_material,
                      {0, 0, 0});

    scene.addInstance(sphere,
                      green_material,
                      {0, 0, 2.5});

    scene.addInstance(sphere,
                      blue_material,
                      {0, 0, -2.5});

    scene.addInstance(plane, {}, {0, -0.5, 0});

    std::mt19937 gen(23451224);
    std::uniform_real_distribution<float> uniform(-1, 1);
    int spp = 1; // samples per pixel

    auto start = std::chrono::high_resolution_clock::now();

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)
                            {
        for(int i = 0 ; i < spp; i++)
        {
            auto ray = camera.generateRay({ x, y },
             {std::abs(uniform(gen)), std::abs(uniform(gen))});
            glm::vec3 beta { 1, 1, 1 };
            glm::vec3 color { 0, 0, 0 };
            while(true)
            {         
                auto hit_info = scene.intersect(ray);
                if (hit_info.has_value()) 
                {
                    color += beta * hit_info->material->getEmissive();
                    beta *= hit_info->material->getAlbedo();

                    ray.origin = hit_info->hit_point;
                    Frame frame (hit_info->normal);
                    glm::vec3 light_direction;
                    if(hit_info->material->getEnableSpecular())
                    {
                        glm::vec3 view_direction = frame.localFromWorld(-ray.direction);
                        light_direction = {-view_direction.x, view_direction.y, view_direction.z};
                    }
                    else
                    {
                        do
                        {
                            light_direction = {uniform(gen), uniform(gen), uniform(gen)};
                        } 
                        while (glm::length(light_direction) > 1.0f);
                        if(light_direction.y < 0)
                        {
                            light_direction.y = -light_direction.y;
                        }
                    }
                    ray.direction = frame.worldFromLocal(light_direction);
                }
                else
                {
                    break;
                }
            }

            film.addSample(x, y, color);

        }


        int n = ++count;
        if (n % film.getWidth() == 0) {
            std::cout << static_cast<float>(n) / (film.getHeight() * film.getWidth()) << std::endl;
        } });

    thread_pool.wait();

    auto end = std::chrono::high_resolution_clock::now();
    // 计算耗时（单位：毫秒）
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Function took " << duration.count() << " ms" << std::endl;

    film.save("test.ppm");
    return 0;
}

/**
 * @brief lambda表达式
        auto ray = camera.generateRay({ x, y });
        auto hit_info = scene.intersect(ray);
        if (hit_info.has_value()) {
            auto L = glm::normalize(light_pos - hit_info->hit_point);
            float cosine = glm::max(0.f, glm::dot(hit_info->normal, L));

            film.setPixel(x, y, { cosine, cosine, cosine });
        }

        int n = ++count;
        if (n % film.getWidth() == 0) {
            std::cout << static_cast<float>(n) / (film.getHeight() * film.getWidth()) << std::endl;
        }
 */
