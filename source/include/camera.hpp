#pragma once

#include "film.hpp"
#include "ray.hpp"

class Camera {
public:
    Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &viewpoint, float fovy);

    Ray generateRay(const glm::ivec2 &pixel_coord, const glm::vec2 &offset = { 0.5, 0.5 }) const;

    Film &getFilm();

    const Film &getFilm() const;

private:
    Film &m_film;
    glm::vec3 m_pos;

    //裁剪空间到相机空间的变换矩阵
    glm::mat4 m_camera_from_clip;
    //相机空间到世界空间的变换矩阵
    glm::mat4 m_world_from_camera;
};