#pragma once

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include<array>

enum NavigationTarget
{
	NAVIGATION_SCENE,
	NAVIGATION_UI
};

class InputManager
{
public:
	static const int MAX_KEYS = 1024;
	static const int MAX_MOUSE_BUTTONS = 8;

private:
	//state
	NavigationTarget navTarget = NAVIGATION_UI;

	//Keyboard keys
	std::array<bool, MAX_KEYS> keysDown;		//keys held down
	std::array<bool, MAX_KEYS> keysPressed;		//keys pressed this frame
	std::array<bool, MAX_KEYS> keysReleased;	//keys released this frame

	//Mouse buttons
	std::array<bool, MAX_MOUSE_BUTTONS> mouseDown;			//keys held down
	std::array<bool, MAX_MOUSE_BUTTONS> mousePressed;		//keys pressed this frame
	std::array<bool, MAX_MOUSE_BUTTONS> mouseReleased;		//keys released this frame

	//Mouse movement
	bool firstMouse;

	glm::vec2 mousePos;
	glm::vec2 lastMousePos;
	glm::vec2 mouseDelta;		//we send this to the Camera

public:
	//Constructors
	InputManager();

	//Destructor
	~InputManager() = default;

	//Callback
	void KeyCallback(int key, int scanCode, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);
	void MouseMoveCallback(double xpos, double ypos);

	//Per-Frame State Reset
	void Update();

	//Getters
	bool IsKeyDown(int key) const		{ return keysDown[key]; }
	bool IsKeyPressed(int key) const	{ return keysPressed[key]; }
	bool IsKeyReleased(int key) const	{ return keysReleased[key]; }

	bool IsMouseDown(int button) const		{ return mouseDown[button]; }
	bool IsMousePressed(int button) const	{ return mousePressed[button]; }
	bool IsMouseReleased(int button) const	{ return mouseReleased[button]; }
	glm::vec2 GetMousePosition() const		{ return mousePos; }
	glm::vec2 GetMouseDelta() const			{ return mouseDelta; }

	NavigationTarget GetNavigationTarget()	{ return navTarget; }

	//setters
	void SetNavigationTarget(NavigationTarget nt) { navTarget = nt; }
};

