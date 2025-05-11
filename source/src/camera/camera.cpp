#include "camera/camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Camera::Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy)
    : m_film(film), m_pos(pos)
{
    m_camera_from_clip = glm::inverse(glm::perspective(
        glm::radians(fovy),
        static_cast<float>(film.getWidth()) / static_cast<float>(film.getHeight()),
        1.f, 2.f));
    m_world_from_camera = glm::inverse(glm::lookAt(pos, viewpoint, {0, 1, 0}));
}

Ray Camera:: generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset) const
{
    glm::vec2 ndc = (glm::vec2(pixel_coord) + offset) / glm::vec2(m_film.getWidth(), m_film.getHeight());
    ndc.y = 1.f - ndc.y;
    // [0, 1] --> [-1, 1]
    ndc = 2.f * ndc - 1.f;

    // {x, y, 0 , near} --> { x /near, y /near, 0}
    glm::vec4 clip{ndc, 0, 1};
    glm::vec3 world = m_world_from_camera * m_camera_from_clip * clip;
    //
    return Ray{
        m_pos,
        glm::normalize(world - m_pos)};
}

Film &Camera::getFilm()
{
    return m_film;
}

const Film &Camera::getFilm() const
{
    return m_film;
}
