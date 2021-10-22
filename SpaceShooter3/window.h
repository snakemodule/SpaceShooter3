#pragma once

#include "GLFW/glfw3.h"

extern GLFWwindow* window;


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void InitWindow();

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
