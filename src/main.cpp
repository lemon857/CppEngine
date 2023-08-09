#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Examples/TanksDemo/TanksDemoGame.h"
#include "Examples/SnakeDemo/SnakeDemoGame.h"
#include "Examples/PongDemo/PongDemoGame.h"

#include "Physics/PhysicsEngine.h"
#include "Engine/Engine.h"


glm::ivec2 g_WindowSize(800, 600);

IGame* g_Game;

void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_Game->setWindowSize(glm::ivec2(width, height));
    RenderEngine::Renderer::setViewport(width, height);
}

void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_Game->setKey(key, action);
}

void Start(int argc, char** argv)
{
    // ������������� ����������� ��������� � ������
    if (!Engine::initGraphics())
    {
        std::cout << "Init graphics failed\n";
        return;
    }

    // �������� ���� ��� ����������� � ��� ������� �����������
    GLFWwindow* pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y,
        "Test Game", nullptr, nullptr);

    if (!pWindow)
    {
        std::cout << "Generate window failed\n";
        glfwTerminate();
        return;
    }

    // ��������� �������� ��� ������������ ��������� �������� ���� � ������� ������
    Engine::setWindowSizeCallBack(pWindow, windowSizeCallback);
    Engine::setKeyCallback(pWindow, keyCallback);

    // ����� �������� ����
    Engine::makeContextCurrent(pWindow);
    // ��������������� OprnGL
    if (!Engine::initOpenGL())
    {
        std::cout << "Init OpenGL failed\n";
        return;
    }

    // ����� ������ OpenGL � ��������� � �������
    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << "\n";
    std::cout << "OpenGL Version: " << RenderEngine::Renderer::getVersionStr() << "\n";
    // ��������� ���� ���������� ��� ��������� �������
    ResourceManager::setExecutablePath(argv[0]);
    // ������������� ������ ����
    if (!g_Game->init())
    {
        std::cout << "Game init failed\n";
        return;
    }
    // �������� ������ ������ ������
    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);
    // ������������� ����������� ������
    Physics::PhysicsEngine::init();
}

void Update(double delta)
{
    g_Game->update(delta);

    Physics::PhysicsEngine::update(delta);

    RenderEngine::Renderer::clearColor();

    g_Game->render();
}

int main(int argc, char** argv)
{
    g_Game = new TanksDemoGame(g_WindowSize);
    //g_Game = new SnakeDemoGame(g_WindowSize, glm::vec2(100));
    //g_Game = new PongDemoGame(g_WindowSize);

    Engine::setStartFunc(Start);
    Engine::setUpdateFunc(Update);
    Engine::startEngine(argc, argv);

    delete g_Game;
    return 0;
}