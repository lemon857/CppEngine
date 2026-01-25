#include "EngineCore/Engine/Window.h"

#include "EngineCore/Editor/UI/UImodule.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/System/Loaders.h"

#include <GLFW/glfw3.h>

const char* gl_source_to_str(const GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: return "DEBUG SOURCE API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG SOURCE WINDOW SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "DEBUG SOURCE SHADER COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG SOURCE THIRD PARTY";
    case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG SOURCE APPLICATION";
    case GL_DEBUG_SOURCE_OTHER: return "DEBUG SOURCE OTHER";
    }
    return "DEBUG SOURCE UNKNOWN";
}
const char* gl_type_to_str(const GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: return "DEBUG TYPE ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG TYPE DEPRECATED BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG TYPE UNDEFINED BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG TYPE PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG TYPE PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER: return "DEBUG TYPE MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG TYPE PUSH GROUP";
    case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG TYPE POP GROUP";
    case GL_DEBUG_TYPE_OTHER: return "DEBUG TYPE OTHER";
    }
    return "DEBUG TYPE UNKNOWN";
}
Window::Window(std::string title, glm::ivec2& window_position, glm::ivec2& window_size, bool maximized, bool fullscreen, std::string path_icon_png)
    : m_data({ std::move(title), window_size, window_position, maximized, fullscreen, nullptr })
    , m_path_icon_png(path_icon_png)
{
	init();
    //UImodule::on_window_create(m_pWindow);
}

Window::~Window()
{
	shuitdown();
}

void Window::on_update()
{
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}

void Window::maximize()
{
    glfwMaximizeWindow(m_pWindow);
}

void Window::set_pos(glm::ivec2& pos)
{
    glfwSetWindowPos(m_pWindow, pos.x, pos.y);
}

void Window::set_cursor_visible(bool isVisible)
{
    glfwSetInputMode(m_pWindow, GLFW_CURSOR, isVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::set_fullscreen(bool isFullscreen)
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(m_pWindow, isFullscreen ? monitor : nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
    if (!isFullscreen)
    {
        glfwSetWindowPos(m_pWindow, m_data.window_position.x, m_data.window_position.y);
        glfwSetWindowSize(m_pWindow, m_data.window_size.x, m_data.window_size.y);
        glfwRestoreWindow(m_pWindow);
        if (m_data.maximized) maximize();
    }
    m_data.fullscreen = isFullscreen;
}

bool Window::is_fullscreen()
{
    return glfwGetWindowMonitor(m_pWindow) != nullptr;
}

void Window::set_vsync(bool enable)
{
    glfwSwapInterval(enable ? 1 : 0);
}

glm::vec2 Window::get_current_cursor_position() const
{
    double x_pos, y_pos;
    glfwGetCursorPos(m_pWindow, &x_pos, &y_pos);
    return glm::vec2(x_pos, y_pos);
}

void Window::set_event_callback(const EventCallback& callback)
{
    m_data.event_callback = callback;
}

int Window::init()
{
    LOG_INFO("Creating window \"{0}\" size {1}x{2}", m_data.title, m_data.window_size.x, m_data.window_size.y);

    RenderEngine::Renderer::init();

    glfwSetErrorCallback(
        [](int error_code, const char* description)
        {
            LOG_CRIT("GLFW Error: {0}: {1}", error_code, description);
        });

    if (!glfwInit())
    {
        LOG_CRIT("Fail init GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
  //glfwWindowHint(GLFW_SAMPLES, 4);

    m_pWindow = glfwCreateWindow(m_data.window_size.x, m_data.window_size.y,
        m_data.title.c_str(), nullptr, nullptr);
   
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    if (!m_pWindow)
    {   
        LOG_CRIT("Generate window failed");
        shuitdown();
        return -1;
    }

    glfwMakeContextCurrent(m_pWindow);
    set_pos(m_data.window_position);
    if (m_data.maximized) maximize();
    if (m_data.fullscreen) set_fullscreen(true);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRIT("Glad load failed");
        return -1;
    }
    // Debug OpenGL
    //glEnable(GL_DEBUG_OUTPUT);
    /*glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}", gl_source_to_str(source), gl_type_to_str(type), id, message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_WARN("OpenGL Warning: [{0}:{1}]({2}): {3}", gl_source_to_str(source), gl_type_to_str(type), id, message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_INFO("OpenGL Info: [{0}:{1}]({2}): {3}", gl_source_to_str(source), gl_type_to_str(type), id, message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_INFO("OpenGL Notification: [{0}:{1}]({2}): {3}", gl_source_to_str(source), gl_type_to_str(type), id, message);
                break;
            default:
                LOG_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}", gl_source_to_str(source), gl_type_to_str(type), id, message);
                break;
            }
        }, nullptr);*/

    if (m_path_icon_png != "")
    {
        GLFWimage icon;
        int channels = 0;
        icon.pixels = loaders::load_image(m_path_icon_png.c_str(), &icon.width, &icon.height, &channels, false);

        if (!icon.pixels) {
            LOG_WARN("Error loading icon window");
        }
        else {
            glfwSetWindowIcon(m_pWindow, 1, &icon);
            loaders::clear_image(icon.pixels);
        }
    }


//#define OFF_CALLBACKS

#ifndef OFF_CALLBACKS
    glfwSetWindowUserPointer(m_pWindow, &m_data);

    glfwSetMouseButtonCallback(m_pWindow, 
        [](GLFWwindow* pWindow, int key, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            double x_pos = 0, y_pos = 0;
            glfwGetCursorPos(pWindow, &x_pos, &y_pos);
            switch (action)
            {
            case GLFW_PRESS:
            {
                EventMouseButtonPressed e(static_cast<MouseButton>(key), x_pos, y_pos);
                data.event_callback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                EventMouseButtonReleased e(static_cast<MouseButton>(key), x_pos, y_pos);
                data.event_callback(e);
                break;
            }
            }
        });

    glfwSetKeyCallback(m_pWindow,
        [](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            switch (action)
            {
            case GLFW_PRESS:
            {
                EventKeyPressed e(static_cast<KeyCode>(key), false);
                data.event_callback(e);
                break;
            }
            case GLFW_REPEAT:
            {
                EventKeyPressed e(static_cast<KeyCode>(key), true);
                data.event_callback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                EventKeyReleased e(static_cast<KeyCode>(key));
                data.event_callback(e);
                break;
            }
            }
            //LOG_INFO("Key: {0} - {1} - {2} - {3}", key, scancode, action, mods);
        });

    glfwSetCharCallback(m_pWindow, [](GLFWwindow* pWindow, unsigned int codepoint) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
        EventCharSet e(static_cast<wchar_t>(codepoint));
        data.event_callback(e);
        });
    glfwSetWindowSizeCallback(m_pWindow, 
        [](GLFWwindow* pWindow, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.window_size = glm::ivec2(width, height);
            EventWindowResize e(width, height);
            data.event_callback(e);
        });
    glfwSetCursorPosCallback(m_pWindow,
        [](GLFWwindow* pWindow, double x, double y)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseMoved e(x, y);
            data.event_callback(e);
        });
    glfwSetScrollCallback(m_pWindow,
        [](GLFWwindow* pWindow, double x, double y)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseScrolled e(x, y);
            data.event_callback(e);
        });    
    glfwSetWindowCloseCallback(m_pWindow,
        [](GLFWwindow* pWindow)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventWindowClose e = EventWindowClose();
            data.event_callback(e);
        });
    glfwSetWindowMaximizeCallback(m_pWindow, 
        [](GLFWwindow* pWindow, int maximized)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMaximizeWindow e = EventMaximizeWindow(maximized == 1 ? true : false);
            data.event_callback(e);
        });
    glfwSetWindowPosCallback(m_pWindow,
        [](GLFWwindow* pWindow, int x_pos, int y_pos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMoveWindow e = EventMoveWindow(x_pos, y_pos);
            data.event_callback(e);
        });

#endif // !OFF_CALLBACKS

    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);
    RenderEngine::Renderer::setStencilTest(true);

	return 0;
}

void Window::shuitdown()
{
    //UImodule::on_window_close();
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}
