#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>


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
	// ��������� ������� ������
	static void setStartFunc(std::function<void(int, char**)> start);
	// ��������� ������� �������
	static void setUpdateFunc(std::function<void(double)> update);
	// ����� �������� ����
	static void makeContextCurrent(GLFWwindow* window);
	// �������� ���������
	static void poolEvents();
	// ������ ������
	static void startEngine(int argc, char** argv);
	// �������� ���� �������� ������ 
	static void terminate();
private:
	static std::function<void(int, char**)> m_start;
	static std::function<void(double)> m_update;
	static GLFWwindow* m_window;

	static int defaultStart(int argc, char** argv);

};