

#include "window.h"

#include <iostream>

GLFWwindow* window;

extern bool ProgramShouldEnd;

void InitWindow()
{
	// Initialise GLFW
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);

	//glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ProgramShouldEnd = true;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
}

void FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
	//auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
	//app->framebufferResized = true;
}