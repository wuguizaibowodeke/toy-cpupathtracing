#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
#include "model/model.hpp"
#include "model/scene.hpp"
#include "material/specular_material.hpp"
#include "material/diffuse_material.hpp"
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
    Camera camera{film, {-12, 5, -12}, {0, 0, 0}, 45};
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

    RandomNumberGenerator rng{1234};

    Scene scene{};

    for (size_t i = 0; i < 10000; ++i)
    {
        float x = rng.uniform() * 100 - 50;
        float y = rng.uniform() * 2;
        float z = rng.uniform() * 100 - 50;
        glm::vec3 pos(x, y, z);
        float u = rng.uniform();

        if (u < 0.9)
        {
            MaterialPtr material;
            if (rng.uniform() > 0.5)
            {
                material = std::make_shared<SpecularMaterial>(RGB(202, 159, 117)) ;
            }
            else
            {
                material = std::make_shared<DiffuseMaterial>(RGB(202, 159, 117));
            }
            scene.addInstance(
                model,
                material,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
        else if (u < 0.95)
        {
            MaterialPtr material = std::make_shared<SpecularMaterial>(glm::vec3{rng.uniform(), rng.uniform(), rng.uniform()});
            scene.addInstance(
                sphere_shape,
                material,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
        else
        {
            pos.y += 6;
            MaterialPtr material = std::make_shared<DiffuseMaterial>(glm::vec3{0, 0, 0});
            material->setEmissive({rng.uniform() * 4, rng.uniform() * 4, rng.uniform() * 4});
            scene.addInstance(
                sphere_shape,
                material,
                pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360});
        }
    }
    MaterialPtr material4 = std::make_shared<DiffuseMaterial>(RGB(120, 204, 157));
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
