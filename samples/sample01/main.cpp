#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
#include "model/model.hpp"
#include "model/scene.hpp"
#include "material/specular_material.hpp"
#include "material/diffuse_material.hpp"
#include "material/dielectric_material.hpp"
#include "material/grid_ground_material.hpp"
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
    Film film{192, 108};
    Camera camera{film, {0, 0, 15}, {0, 0, 0}, 90};
    std::atomic<int> count = 0;

    auto model = std::shared_ptr<Model>(new Model("asset/models/dragon_871k.obj"));
    Sphere sphere{
        {0, 0, 0},
        1.0f};
    ShapePtr sphere_shape = std::make_shared<Sphere>(sphere);

    Plane planeleft{
        {0, 0, 0},
        {-1, 0, 0}};
    ShapePtr plane_shape_left = std::make_shared<Plane>(planeleft);

    Plane planeback{
        {0, 0, 0},
        {0, 0, 1}};
    ShapePtr plane_shape_back = std::make_shared<Plane>(planeback);

    Plane planeup{
        {0, 0, 0},
        {0, 1, 0}};
    ShapePtr plane_shape_up = std::make_shared<Plane>(planeup);

    MaterialPtr whiteMat = std::make_shared<DiffuseMaterial>(RGB(255, 255, 255));
    MaterialPtr redMat = std::make_shared<DiffuseMaterial>(RGB(255, 0, 0));
    MaterialPtr greenMat = std::make_shared<DiffuseMaterial>(RGB(0, 255, 0));

    RandomNumberGenerator rng{1234};

    Scene scene{};
    scene.addInstance(
        plane_shape_left,
        redMat,
        {-5, 0, 0});
    scene.addInstance(
        plane_shape_left,
        greenMat,
        {5, 0, 0});
    scene.addInstance(
        plane_shape_back,
        whiteMat,
        {0, 0, -5});
    scene.addInstance(
        plane_shape_up,
        whiteMat,
        {0, 5, 0});
    scene.addInstance(
        plane_shape_back,
        whiteMat,
        {0, -5, 0});

    MaterialPtr lightMat = std::make_shared<DiffuseMaterial>(glm::vec3(1, 1, 1));
    lightMat->setEmissive(glm::vec3(1, 1, 1));
    scene.addInstance(
        sphere_shape,
        lightMat,
        {0, 0, 0},
        {2.0, 2.0 ,2.0});

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
