#include <iostream>
#include "glm/glm.hpp"
#include "film.hpp"
#include "thread_pool.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "shape.hpp"
#include "model.hpp"

const int WIDTH = 192;
const int HEIGHT = 108;

int main() {
    std::ios_base::sync_with_stdio(false);
    Film film(WIDTH, HEIGHT);
    Camera camera(film, { -1, 0, 0 }, { 0, 0, 0}, 90.0f);
    Sphere sphere{ { 0, 0, 0 }, 0.5f };

    Model model("asset/models/simple_dragon.obj");
    Shape &shape = model;
    glm::vec3 light_pos { -1, 2, 1 };

    auto start = std::chrono::high_resolution_clock::now();
    std::atomic<int> count = 0;
    ThreadPool thread_pool {};
    thread_pool.parallelFor(WIDTH, HEIGHT, [&](size_t x, size_t y) {
       
        auto ray = camera.generateRay({ x, y });
        auto hitinfo = shape.intersect(ray);
        if(hitinfo.has_value())
        {
            auto L = glm::normalize(light_pos - hitinfo->hit_point);
            float cosine = glm::max(0.f, glm::dot(hitinfo->normal, L));
            film.setPixel(x, y, { cosine, cosine, cosine });
            count++;
            if (count % film.getWidth() == 0) {
                std::cout << "Progress: " << static_cast<float>(count) / (film.getHeight() * film.getWidth()) << std::endl;
            }
        }
    });
    thread_pool.wait();
    auto end = std::chrono::high_resolution_clock::now();
    // 计算耗时（单位：毫秒）
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Function took " << duration.count() << " ms" << std::endl;

    film.save("output.ppm"); // Save the film to a PPM fil

    return 0;
}