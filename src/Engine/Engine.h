#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine
{
public:
	// ������������� �������
	// �����: 
	// false ��� ��������� ������������
	// true ��� ������� �������������
	static bool initGraphics();
	// ������������� OpenGL
	// �����: 
	// false ��� ��������� ������������
	// true ��� ������� �������������
	static bool initOpenGL();
	// ��������� �������� ��� ������������ ��������� �������� ����
	static void setWindowSizeCallBack(GLFWwindow* window, GLFWwindowsizefun callback);
	// ��������� ��������� ��� ������������ ������� ������
	static void setKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
	// ����� �������� ����
	static void makeContextCurrent(GLFWwindow* window);
	// �������� ���������
	static void poolEvents();
	// �������� ���� �������� ������ 
	static void terminate();
private:

};