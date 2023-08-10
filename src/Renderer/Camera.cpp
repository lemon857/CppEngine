#include "EngineCore/Renderer/Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode mode)
	: m_position(position)
	, m_rotation(rotation)
	, m_projection_mode(mode)
{
	update_veiw_matrix();
	update_projection_matrix();
}

void Camera::set_position(const glm::vec3& position)
{
	m_position = position;
	update_veiw_matrix();
}

void Camera::set_rotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	update_veiw_matrix();
}

void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
{
	m_position = position;
	m_rotation = rotation;
	update_veiw_matrix();
}

void Camera::set_projection_mode(const ProjectionMode mode)
{
	m_projection_mode = mode;
	update_projection_matrix();
}
void Camera::move_forward(const float delta)
{
	m_position += m_direction * delta;
	update_veiw_matrix();
}
void Camera::move_right(const float delta)
{
	m_position += m_right * delta;
	update_veiw_matrix();
}
void Camera::move_up(const float delta)
{
	m_position += m_up * delta;
	update_veiw_matrix();
}
void Camera::add_movement_and_rotate(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
{
	m_position += m_direction * movement_delta.x;
	m_position += m_right * movement_delta.y;
	m_position += m_up * movement_delta.z;
	m_rotation += rotation_delta;
	update_veiw_matrix();
}
void Camera::update_veiw_matrix()
{
	const float roll_radians = glm::radians(-m_rotation.x);
	const float pitch_radians = glm::radians(-m_rotation.y);
	const float yaw_radians = glm::radians(-m_rotation.z);

	const glm::mat3 rotate_matrix_x(
		1, 0, 0, 
		0, cos(roll_radians), sin(roll_radians),
		0, -sin(roll_radians), cos(roll_radians));

	const glm::mat3 rotate_matrix_y(
		cos(pitch_radians), 0, -sin(pitch_radians),
		0, 1, 0,
		sin(pitch_radians), 0, cos(pitch_radians));

	const glm::mat3 rotate_matrix_z(
		cos(yaw_radians), sin(yaw_radians), 0.f, 
		-sin(yaw_radians), cos(yaw_radians), 0.f,
		0.f, 0.f, 1.f);

	const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

	m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
	m_right = glm::normalize(euler_rotate_matrix * s_world_rigt);
	m_up = glm::cross(m_right, m_direction);

	m_veiw_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::update_projection_matrix()
{
	if (m_projection_mode == ProjectionMode::Perspective)
	{
		float r = 0.1f;
		float t = 0.1f;
		float f = 10;
		float n = 0.1f;
		m_projection_matrix = glm::mat4(
			n / r, 0, 0, 0,
			0, n / t, 0, 0,
			0, 0, (-f - n) / (f - n), -1,
			0, 0, -2 * f * n / (f - n), 0
		);
	}
	else
	{
		float r = 2;
		float t = 2;
		float f = 100;
		float n = 0.1f;
		m_projection_matrix = glm::mat4(
			1 / r, 0, 0, 0,
			0, 1 / t, 0, 0,
			0, 0, -2 / (f - n), 0,
			0, 0, (-f - n) / (f - n), 1
		);
	}
}