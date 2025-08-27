#include "Engine.h"


//=============================================================================================
//Destructor
//=============================================================================================

Engine::~Engine()
{
	//Terminate GLFW
	glfwTerminate();
}
//=============================================================================================
//InitGLFW
//=============================================================================================
void Engine::Init()
{
	engineState = Engine_Running;

	InitGLFW();
	InitGLAD();
	InitOpenGlState();

	imGuiLayer = std::make_unique<ImGuiLayer>(window);
}

//=============================================================================================
//InitGLFW
//=============================================================================================
void Engine::InitGLFW()
{
	//initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFW window creation
	//set window pointer to this Engine instance
	window = glfwCreateWindow(globalScreenWidth, globalScreenHeight, "OPENGL", NULL, NULL);
	glfwSetWindowUserPointer(window, this);

	if (window == NULL) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);

	//set up callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	//tell glfw to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
//=============================================================================================
//InitGLFW
//=============================================================================================

void Engine::InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		throw std::runtime_error("Failed to initialize GLAD");
	}
}
//=============================================================================================
//InitOpenGlState
//=============================================================================================

void Engine::InitOpenGlState()
{
	//Depth testing
	glEnable(GL_DEPTH_TEST);

	//Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Initialize engine objects
	scene = std::make_unique<Scene>(globalScreenWidth, globalScreenHeight);
	renderer = std::make_unique<Renderer>(globalScreenWidth, globalScreenHeight);
	inputManager = std::make_unique<InputManager>();

	//Configure viewport
	glViewport(0, 0, globalScreenWidth, globalScreenHeight);

	Utils::getOpenGLError("ENGINE::INIT_OPENGL");
}
//=============================================================================================
//framebuffer_size_callback
//=============================================================================================

void Engine::OnWindowResize(int width, int height)
{
	glViewport(0, 0, width, height);
}

//=============================================================================================
//Run
//=============================================================================================

void Engine::Run()
{
	while (engineState != Engine_Exit)
	{
		//Per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//PROCESS INPUT
		glfwPollEvents();
		ProcessInput();

		//UPDATE
		scene->UpdateScene();

		//RENDER
		renderer->Draw(*scene.get());

		//IMGUI
		imGuiLayer->Render();

		//SWAP BUFFERS
		glfwSwapBuffers(window);

		//reset key per-frame states
		inputManager->Update();		
	}
}
//=============================================================================================
//ProcessInput
//=============================================================================================

void Engine::ProcessInput()
{
	//Exit application
	if (inputManager->IsKeyPressed(GLFW_KEY_ESCAPE))
		Exit();

	//Moving around
	if (inputManager->IsKeyDown(GLFW_KEY_W))
		scene->SetCameraMovement(FORWARD, deltaTime);
	if (inputManager->IsKeyDown(GLFW_KEY_S))
		scene->SetCameraMovement(BACKWARD, deltaTime);
	if (inputManager->IsKeyDown(GLFW_KEY_A))
		scene->SetCameraMovement(LEFT, deltaTime);
	if (inputManager->IsKeyDown(GLFW_KEY_D))
		scene->SetCameraMovement(RIGHT, deltaTime);

	//Sprinting
	if (inputManager->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		scene->SetCameraSprint(true);
	if (inputManager->IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		scene->SetCameraSprint(false);
	
	Utils::getOpenGLError("ENGINE::PROCESS_INPUT");
}
