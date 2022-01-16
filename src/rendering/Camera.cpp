#include "rendering/Camera.h"

#include "Util.h"
#include <glm/gtc/matrix_access.hpp>

Camera::Camera()
{
    reset();
}

Camera::~Camera()
{
}

void Camera::reset()
{
     // TODO what do I reset?
     m_position = glm::vec3();
     m_orientation = glm::quat(1, 0, 0, 0);

     update();
}

void Camera::update()
{
    m_position = lerp(m_position, m_positionTarget, m_positionDrag);
    //m_orientation = glm::slerp(m_orientation, m_orientationTarget, m_orientationDrag);
}

void Camera::move(const glm::vec3& delta)
{
    m_positionTarget += delta;
    m_updateNeeded = true;
}

void Camera::moveRel(const glm::vec3& delta)
{
    glm::vec3 realDelta =
        delta * right() +
        delta * up() +
        delta * forward();

    move(realDelta);
}

void Camera::rotate(const glm::vec3& axis, float angle)
{
    // TODO m_orientationTarget slerp?
    glm::quat rot = glm::normalize(glm::angleAxis(angle, axis));

    m_orientation = m_orientation * rot;
    m_updateNeeded = true;
}

const glm::vec3 Camera::right() const {
    return glm::vec3(glm::row(rotation(), 0));
}

const glm::vec3 Camera::up() const {
    return glm::vec3(glm::row(rotation(), 1));
}

const glm::vec3 Camera::forward() const {
    return glm::vec3(glm::row(rotation(), 2));
}

glm::mat4 Camera::matrix() const
{
    return projection() * view();
}

glm::mat4 Camera::view() const
{
    return rotation() * translation();
}

glm::mat4 Camera::projection() const {
	return glm::perspective(m_fov, m_aspectRatio, m_znear, m_zfar);
}

glm::mat4 Camera::translation() const {
    return glm::translate(glm::mat4(1.0f), -m_position);

}

#include <iostream>
glm::mat4 Camera::rotation() const {
    std::cout << m_orientation.w << ", " << m_orientation.x << ", " << m_orientation.y << ", " << m_orientation.z << ", " << std::endl;

    return glm::toMat4(m_orientation);
}
