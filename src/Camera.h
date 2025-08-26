#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Mode {
	FPS,
	FLIGHT
};

//Default values
const float yaw_def			= -90.0f;
const float pitch_def		= 0.0f;
const float speed_def		= 2.5f;
const float sensitivity_def = 0.1f;
const float zoom_def		= 45.0f;

//An abstract camera class that processes input and claculates matching Euler angles,
//vectors and matrices for OpenGL

class Camera
{
private:

	void update_camera_vectors();

public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//Euler angles
	float yaw;
	float pitch;
	//camera options
	Camera_Mode camera_mode;
	float mouse_sensitivity;
	float zoom;
	float movement_speed;
	float sprint_speed;
	float sprint_factor = 5.0f;

	//Constructors
	//------------
	Camera(Camera_Mode cam_mode, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw_in = yaw_def, float pitch_in = pitch_def);
	Camera(Camera_Mode cam_mode, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float upX = 0.0f, float upY = 0.0f, float upZ = 0.0f, float yaw_in = yaw_def, float pitch_in = pitch_def);

	//methods
	//-------
	glm::mat4 get_view_matrix() const;
	void process_keyboard(Camera_Movement direction, float deltaTime);
	void process_mouse_scroll(float y_offset);
	void process_mouse_movement(glm::vec2 delta, GLboolean constrainPitch = true);
	glm::vec3 move_position_front_vec(float velocity);
	void is_sprinting(bool set) { set ? movement_speed = sprint_speed : movement_speed = speed_def; }

};

