#include "Camera.h"


//======================================
// Constructors
//======================================

Camera::Camera(Camera_Mode cam_mode, glm::vec3 position, glm::vec3 up, float yaw_in, float pitch_in)
	:Front{ glm::vec3(0.0f, 0.0f, -1.0f) }, movement_speed{ speed_def }, mouse_sensitivity{ sensitivity_def }, zoom{ zoom_def }, camera_mode{cam_mode}
{
	Position = position;
	WorldUp = up;
	yaw = yaw_in;
	pitch = pitch_in;
	sprint_speed = movement_speed * sprint_factor;

	update_camera_vectors();
}

Camera::Camera(Camera_Mode cam_mode, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_in, float pitch_in)
	:Front{ glm::vec3(0.0f, 0.0f, -1.0f) }, movement_speed{ speed_def }, mouse_sensitivity{ sensitivity_def }, zoom{ zoom_def }, camera_mode{ cam_mode } {

	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	yaw = yaw_in;
	pitch = pitch_in;
	sprint_speed = movement_speed * 1.5f;

	update_camera_vectors();
}

//======================================
// get_view_matrix
//======================================

glm::mat4 Camera::get_view_matrix() const
{
	//first arg sets the camera position
	//second arg is the direction, defined by the current pos plus the direction vec (cameraFront)
	return glm::lookAt(Position, Position + Front, Up);
}

//======================================
// process_keyboard
//======================================

void Camera::process_keyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movement_speed * deltaTime;

	switch (direction) {
		case FORWARD:
			Position += move_position_front_vec(velocity);
			break;
		case BACKWARD:
			Position -= move_position_front_vec(velocity);
			break;
		case LEFT:
			Position -= Right * velocity;
			break;
		case RIGHT:
			Position += Right * velocity;
			break;
		default:
			break;
	}
}

glm::vec3 Camera::move_position_front_vec(float velocity) {
	if (camera_mode == FLIGHT)

		return Front * velocity;

	else {

		glm::vec3 xz_front = glm::vec3(Front.x, 0.0f, Front.z);
		xz_front = glm::normalize(xz_front);
		return xz_front * velocity;
	}
}

//======================================
// process_mouse_scroll
//======================================

void Camera::process_mouse_scroll(float y_offset)
{
	zoom -= (float)y_offset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

//======================================
// process_mouse_movement
//======================================

void Camera::process_mouse_movement(glm::vec2 delta, GLboolean constrainPitch)
{
	//we adjust the offset based on the sensitivity value
	delta.x *= mouse_sensitivity;
	delta.y *= mouse_sensitivity;

	yaw		+= delta.x;
	pitch	+= delta.y;

	//we constrain the up/down camera movement
	if (constrainPitch) {

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	//update Front, Right, Up vectors using the updated euler angles
	update_camera_vectors();
}

//======================================
// update_camera_vectors
//======================================

void Camera::update_camera_vectors()
{
	//calculate the new Front vector
	glm::vec3 front_new;
	front_new.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_new.y = sin(glm::radians(pitch));
	front_new.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(front_new);

	//also re-calculate the Right and Up vector
	Right	= glm::normalize(glm::cross(Front, WorldUp));
	Up		= glm::normalize(glm::cross(Right, Front));
}

