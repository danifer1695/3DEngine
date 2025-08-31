#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Transform
{
protected:
	//State
	bool dirty = false;		//Tells the renderer whether it should update the shadows

	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 rotation	= glm::vec3(0.0f);	//Euler angles
	glm::vec3 scale		= glm::vec3(1.0f);

public:
	//Position
	void SetPosition(const glm::vec3& pos) { position = pos;		dirty = true;}
	void Translate(const glm::vec3& delta) { position += delta;		dirty = true;}

	//Rotation
	void SetRotation(const glm::vec3& rot)					{ rotation = rot;					dirty = true;}
	void SetRotation(float pitch, float yaw, float roll)	{ rotation = { pitch, yaw, roll };	dirty = true;}
	void Rotate(const glm::vec3& delta)						{ rotation += delta;				dirty = true;}

	//Scale
	void SetScale(const glm::vec3& scl)		{ scale = scl;					dirty = true; }
	void SetScale(float scl)				{ scale = { scl, scl, scl };	dirty = true; }
	void Scale(const glm::vec3& factor)		{ scale *= factor;				dirty = true; }

	//Getters
	const glm::vec3& getPosition() const { return position; }
	const glm::vec3& getRotation() const { return rotation; }
	const glm::vec3& getScale() const { return scale; }

	glm::mat4 GetModelMatrix() const;
	const bool GetIsDirty() const { return dirty; }

	//Setters
	void SetIsDirty(bool set) { dirty = set; }
};

