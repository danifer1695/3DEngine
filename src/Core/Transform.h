#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Transform
{
protected:
	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 rotation	= glm::vec3(0.0f);	//Euler angles
	glm::vec3 scale		= glm::vec3(1.0f);

public:
	//Position
	void SetPosition(const glm::vec3& pos) { position = pos; }
	void Translate(const glm::vec3& delta) { position += delta; }

	const glm::vec3& getPosition() const { return position; }

	//Rotation
	void SetRotation(const glm::vec3& rot)					{ rotation = rot; }
	void SetRotation(float pitch, float yaw, float roll)	{ rotation = { pitch, yaw, roll }; }
	void Rotate(const glm::vec3& delta)						{ rotation += delta; }

	const glm::vec3& getRotation() const { return rotation; }

	//Scale
	void SetScale(const glm::vec3& scl)		{ scale = scl; }
	void SetScale(float scl)				{ scale = { scl, scl, scl }; }
	void Scale(const glm::vec3& factor)		{ scale *= factor; }

	const glm::vec3& getScale() const { return scale; }

	//Model matrix
	glm::mat4 GetModelMatrix() const;
};

