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

#include <iostream>
#include <random>

#include "log.hpp"

int main()
{
    Logger::Init();
    Film film{192 * 4, 108 * 4};
    Camera camera{film, {3.6, 0, 0}, {0, 0, 0}, 45};
    std::atomic<int> count = 0;

    Model model("asset/models/dragon_87k.obj");
    //Model model("asset/models/simple_dragon.obj");
    Sphere sphere{
        {0, 0, 0},
        1.0f};
    Plane plane{
        {0, 0, 0},
        {0, 1, 0}};

    RGB color1(202, 159, 117);
    RGB color2(255, 128, 128);
    RGB color3(128, 128, 255);

    RGB r = RGB(255, 0, 0);


    Material red_material{color1, {0, 0, 0} , false};
    Material green_material{{1,1,1},color2, false};
    Material blue_material{{1,1,1},color3, true};

    Scene scene {};
    scene.addInstance(
        model,
        red_material,
        { 0, 0, 0 },
        { 1, 3, 2 }
    );
    
    
    scene.addInstance(
        sphere,
        green_material,
        { 0, 0, 2.5 }
    );
    scene.addInstance(
        sphere,
        blue_material,
        { 0, 0, -2.5 }
    );

    NormalRenderer normal_renderer{camera, scene};
    normal_renderer.render(1, "normal.ppm");

    const size_t spp = 16;

#if(1)
    BoundsTestCountRenderer bounds_test_count_renderer{camera, scene};
    bounds_test_count_renderer.render(1, "bounds_test_count.ppm");

    TriangleTestCountRenderer triangle_test_count_renderer{camera, scene};
    triangle_test_count_renderer.render(1, "triangle_test_count.ppm");

    BoundsDepthRenderer bounds_depth_renderer{camera, scene};
    bounds_depth_renderer.render(1, "bounds_depth.ppm");
#endif

    SimpleRTRenderer simple_rt_renderer{camera, scene};
    simple_rt_renderer.render(spp, "simple_rt.ppm");

}
