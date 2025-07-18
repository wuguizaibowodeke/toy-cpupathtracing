#include "renderers/baserenderer.hpp"
#include "thread/thread_pool.hpp"   
#include "util/progress.hpp"
#include "util/profile.hpp"
#include "log.hpp"

BaseRenderer::BaseRenderer(Camera &camera, Scene &scene)
: m_camera(camera), m_scene(scene)
{
}

void BaseRenderer::render(size_t spp, const std::filesystem::path& path)
{
    PROFILE("Render " + std::to_string(spp) + "spp " + path.string())
    size_t current_spp = 0 , increase = 1;
    auto &film = m_camera.getFilm();
    film.clear();
    Progress progress{film.getWidth() * spp * film.getHeight() * spp, 20};
    while(current_spp < spp)
    {
        auto signalPixelColor = [&](size_t x, size_t y)
        {
            for(int i = 0; i < increase; i++)
            {
                film.addSample(x, y, renderPixel({x, y}));
            }
            progress.update(increase);
        };
        thread_pool.parallelFor(film.getWidth(), film.getHeight(), signalPixelColor);
        thread_pool.wait();
        current_spp += increase;
        increase = std::min<size_t>(current_spp, 32);

        film.save(path);
        //std::cout << current_spp << "spp has been saved to " << path << std::endl;
        LOG_I("Render {} spp has been saved to {}", current_spp, path.string());
    }
}