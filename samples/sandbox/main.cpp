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

#include <iostream>
#include <random>

int main()
{
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

    RGB color1(202, 159, 117);
    RGB color2(255, 128, 128);
    RGB color3(128, 128, 255);

    Material red_material{glm::vec3{1, 1, 1}, color1, false};
    Material green_material{glm::vec3{1, 1, 1}, color2, false};
    Material blue_material{glm::vec3{1, 1, 1}, color3, false};

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

    scene.addInstance(plane, {glm::vec3{1, 1, 1},RGB(120, 204, 157),false}, {0, -0.5, 0});

    NormalRenderer normal_renderer { camera, scene };
    normal_renderer.render(1, "normal.ppm");

    film.clear();

    SimpleRTRenderer simple_rt_renderer { camera, scene };
    simple_rt_renderer.render(32, "simple_rt.ppm");
}


