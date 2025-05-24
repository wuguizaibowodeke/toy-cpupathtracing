#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
#include "model/model.hpp"
#include "model/scene.hpp"
#include "model/material.hpp"
#include "util/frame.hpp"
#include "util/rgb.hpp"
#include "util/progress.hpp"
#include "renderers/normal_renderer.hpp"
#include "renderers/simple_rt_renderer.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderers/pathtracing_renderer.hpp"

#include <iostream>
#include <random>
#include <memory>

#include "log.hpp"

int main()
{
    Logger::Init();
    Film film{192 * 4, 108 * 4};
    Camera camera{film,  { -12, 5, -12 }, { 0, 0, 0 }, 45 };
    std::atomic<int> count = 0;

    auto model = std::shared_ptr<Model>(new Model("asset/models/dragon_871k.obj"));
    Sphere sphere{
        {0, 0, 0},
        1.0f};

    ShapePtr sphere_shape = std::make_shared<Sphere>(sphere);

    Plane plane{
        {0, 0, 0},
        {0, 1, 0}};
    ShapePtr plane_shape = std::make_shared<Plane>(plane);

    RGB color1(202, 159, 117);
    RGB color2(255, 128, 128);
    RGB color3(128, 128, 255);

    RGB r = RGB(255, 0, 0);

    Material material{color1, {0, 0, 0}, false};
    Material green_material{{1, 1, 1}, color2, false};
    Material blue_material{{1, 1, 1}, color3, true};

    RandomNumberGenerator rng{1234};

    Scene scene{};
    
    for (size_t i = 0; i < 10000; ++i)
    {
        float x = rng.uniform() * 100 - 50;
        float y = rng.uniform() * 2;
        float z = rng.uniform() * 100 - 50;
        glm::vec3 pos(x, y, z);
        float u = rng.uniform();
        auto material1 = std::make_shared<Material>(RGB(202, 159, 117), glm::vec3(0, 0, 0), rng.uniform() > 0.5);

        auto material2 = std::make_shared<Material>(RGB(rng.uniform() * 255, rng.uniform() * 255, rng.uniform() * 255), glm::vec3(0, 0, 0), true);

        auto material3 = std::make_shared<Material>(glm::vec3{1, 1 ,1}, glm::vec3(rng.uniform() * 4, rng.uniform() * 4, rng.uniform() * 4), false);
  
        if (u < 0.9)
        {
            scene.addInstance(
                model,
                material1,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
        else if (u < 0.95)
        {
            scene.addInstance(
                sphere_shape,
                material2,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
        else
        {
            pos.y += 6;
            scene.addInstance(
                sphere_shape,
                material3,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
    }
    auto material4 = std::make_shared<Material>(RGB(120, 204, 157), glm::vec3(0, 0, 0), false);
    scene.addInstance(
        plane_shape,
        material4,
        {0, -2, 0},
        {10, 10, 10},
        {0, 0, 0});

    scene.buildBVH();

    NormalRenderer normal_renderer{camera, scene};
    normal_renderer.render(1, "normal.ppm");

    const size_t spp = 128;

    BoundsTestCountRenderer bounds_test_count_renderer{camera, scene};
    bounds_test_count_renderer.render(1, "bounds_test_count.ppm");

    TriangleTestCountRenderer triangle_test_count_renderer{camera, scene};
    triangle_test_count_renderer.render(1, "triangle_test_count.ppm");

    SimpleRTRenderer simple_rt_renderer{camera, scene};
    simple_rt_renderer.render(spp, "simple_rt.ppm");

    PathTracingRenderer path_tracing_renderer{camera, scene};
    path_tracing_renderer.render(spp, "path_tracing.ppm");
}
