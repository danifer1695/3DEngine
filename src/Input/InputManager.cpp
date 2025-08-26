#include "InputManager.h"

//=============================================================================================
//Constructor
//=============================================================================================
InputManager::InputManager()
	:mousePos(0.0f), lastMousePos(0.0f), mouseDelta(0.0f), firstMouse(true)
{
	//fill the array with 'false' values
	keysDown.fill(false);
	keysPressed.fill(false);
	keysReleased.fill(false);

	mouseDown.fill(false);
	mousePressed.fill(false);
	mouseReleased.fill(false);
}
//=============================================================================================
//KeyCallback
//=============================================================================================

void InputManager::Update() {

	// clear per-frame states
	keysPressed.fill(false);
	keysReleased.fill(false);

	mousePressed.fill(false);
	mouseReleased.fill(false);

	// reset mouse delta
	mouseDelta = glm::vec2(0.0f);
}
//=============================================================================================
//KeyCallback
//=============================================================================================

void InputManager::KeyCallback(int key, int scanCode, int action, int mods)
{
	//check input value does not exceed key arrays size
	if (key < 0 || key >= MAX_KEYS) return;

	if (action == GLFW_PRESS)
	{
		keysDown[key] = true;
		keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keysDown[key] = false;
		keysReleased[key] = true;
	}
}
//=============================================================================================
//MouseButtonCallback
//=============================================================================================

void InputManager::MouseButtonCallback(int button, int action, int mods)
{
	//check input value does not exceed button arrays size
	if (button < 0 || button >= MAX_MOUSE_BUTTONS) return;

	if (action == GLFW_PRESS)
	{
		mouseDown[button] = true;
		mousePressed[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		mouseDown[button] = false;
		mouseReleased[button] = true;
	}
}
//=============================================================================================
//MouseMoveCallback
//=============================================================================================

void InputManager::MouseMoveCallback(double xpos, double ypos)
{
	mousePos = glm::vec2((float)xpos, (float)ypos);

	if (firstMouse) {
		lastMousePos = mousePos;
		firstMouse = false;
	}

	mouseDelta = mousePos - lastMousePos;
	mouseDelta.y *= -1.0;					//y coords go from top to bottom so we reverse y coords

	lastMousePos = mousePos;
}
