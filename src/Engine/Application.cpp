#include "EngineCore/Engine/Application.h"
#include "EngineCore/Engine/Window.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/System/Stopwatch.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Sound/SoundEngine.h"
#include "EngineCore/System/Input.h"

#include <iostream>
#include <thread>

Application::Application()
{}

Application::~Application()
{}

int Application::start(glm::ivec2& window_size, const char* title, const char* json_rel_path, const char* ini_rel_path, const char* icon_rel_path, double tps_max)
{
    LogSystem::init_log_system("logs");
    //LOG_INFO("Build name: {0}", BUILD_NAME);
    LOG_INFO("Starting Application");

    m_watch = new Stopwatch();

    m_window_size = window_size;

    INIregionSTARTUP startup{ &m_window_size, &m_window_position, &m_maximized_window, &m_fullscreen_window };

    m_ini_data.add_region("STARTUP", &startup);

    ResourceManager::load_INI_settings(ini_rel_path, m_ini_data, false);

    m_pCloseWindow = false;
    m_pWindow = 
        std::make_unique<Window>(title, m_window_position, m_window_size, 
            m_maximized_window, m_fullscreen_window, icon_rel_path == 0 ? "" : ResourceManager::getExeFilePath() + icon_rel_path);

    LOG_INFO("Renderer: {0}", RenderEngine::Renderer::getRendererStr());
    LOG_INFO("OpenGL version: {0}", RenderEngine::Renderer::getVersionStr());

    if (SoundEngine::init_audio() != 0) LOG_ERROR("Fail init sound engine");

    ResourceManager::load_JSON_resources(json_rel_path);

    init_system_events();

    init_events();
    if (!init())
    {
        return -1;
    }
    
    LOG_INFO("Time initialization: {0}", m_watch->stop());
    LOG_INFO("==========================================");
    
    delete m_watch;

    m_count_fps_check = 50;
    m_count_tps_check = 10;
    m_current_tps = 0;
    m_current_fps = 0;
    m_time_tick = 0;
    m_ticks = 0;
    m_time_frame = 0;
    m_frames = 0;

    auto lastTime = std::chrono::high_resolution_clock::now();      

    double sum_time = 0;

    m_max_time_tps = 1000.0 / (tps_max + 1);

    while (!m_pCloseWindow)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        sum_time += duration;

        if (sum_time >= m_max_time_tps)
        {
//#ifdef DEBUG
            on_update(sum_time);
//#else
//            on_update(sum_time);
//#endif
            // tps counter
            if (m_ticks < m_count_tps_check)
            {
                m_ticks++;
                m_time_tick += sum_time;
            }
            else
            {
                m_current_tps = int((m_ticks / m_time_tick) * 1000.f);
                m_ticks = 0;
                m_time_tick = 0;
            }
            sum_time = 0;
        }

        on_render();

        on_ui_render();
        on_key_update(duration);

        m_pWindow->on_update();    

        // fps counter
        if (m_frames < m_count_fps_check)
        {
            m_frames++;
            m_time_frame += duration;
        }
        else
        {
            m_current_fps = int((m_frames / m_time_frame) * 1000.f);
            m_frames = 0;
            m_time_frame = 0;
        }
    }

    m_fullscreen_window = m_pWindow->is_fullscreen();
    m_window_size = m_pWindow->get_size();

    ResourceManager::load_INI_settings(ini_rel_path, m_ini_data, true);

    m_pWindow = nullptr;

    return 0;
}

void Application::stop()
{    
    LOG_INFO("Closing Application");
    terminate();
    ResourceManager::unloadAllResources();
    SoundEngine::uninit_audio();
    LogSystem::uninit_log_system();
}

void Application::set_max_tps(double max)
{
    m_max_time_tps = 1000.0 / (max + 1);
}

void Application::init_system_events()
{
    m_system_event_dispather.add_event_listener<EventWindowResize>([&](EventWindowResize& e)
        {
            if (e.width != 0 && e.height != 0)
            {
                RenderEngine::Renderer::setViewport(e.width, e.height);
                m_event_dispather.dispatch(e);
            }
        });
    m_system_event_dispather.add_event_listener<EventKeyPressed>([&](EventKeyPressed& e)
        {
            m_event_dispather.dispatch(e);
            Input::pressKey(e.key_code);
        });
    m_system_event_dispather.add_event_listener<EventCharSet>([&](EventCharSet& e)
        {
            m_event_dispather.dispatch(e);
        });
    m_system_event_dispather.add_event_listener<EventKeyReleased>([&](EventKeyReleased& e)
        {
            m_event_dispather.dispatch(e);
            Input::releaseKey(e.key_code);
        });
    m_system_event_dispather.add_event_listener<EventMouseMoved>([&](EventMouseMoved& e)
        {
            m_event_dispather.dispatch(e);
        });
    m_system_event_dispather.add_event_listener<EventMouseScrolled>([&](EventMouseScrolled& e)
        {
            m_event_dispather.dispatch(e);
        });
    m_system_event_dispather.add_event_listener<EventWindowClose>([&](EventWindowClose& e)
        {
            m_event_dispather.dispatch(e);
            m_pCloseWindow = true;
        });
    m_system_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
        {
            m_event_dispather.dispatch(e);
            Input::pressMouseButton(e.mouse_button);
        });
    m_system_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            m_event_dispather.dispatch(e);
            Input::releaseMouseButton(e.mouse_button);
        });
    m_system_event_dispather.add_event_listener<EventMaximizeWindow>([&](EventMaximizeWindow& e)
        {
            m_maximized_window = e.isMaximized;
            m_event_dispather.dispatch(e);
        });
    m_system_event_dispather.add_event_listener<EventMoveWindow>([&](EventMoveWindow& e)
        {
            m_window_position = glm::ivec2(e.x_pos, e.y_pos);
            m_event_dispather.dispatch(e);
        });
    m_pWindow->set_event_callback(
        [&](BaseEvent& e)
        {
            m_system_event_dispather.dispatch(e);
        });
}

void INIregionSTARTUP::parse(std::string name, std::string value)
{
    if (name == "window_width") window_size->x = std::stoi(value);
    else if (name == "window_height") window_size->y = std::stoi(value);
    else if (name == "window_position_x") window_position->x = std::stoi(value);
    else if (name == "window_position_y") window_position->y = std::stoi(value);
    else if (name == "window_maximized") *maximized_window = value == "true";
    else if (name == "window_fullscreen") *fullscreen = value == "true";
}

std::string INIregionSTARTUP::get_str_data()
{
    std::string data = "";
    data += "window_width=" + std::to_string(window_size->x) + "\n";
    data += "window_height=" + std::to_string(window_size->y) + "\n";
    data += "window_position_x=" + std::to_string(window_position->x) + "\n";
    data += "window_position_y=" + std::to_string(window_position->y) + "\n";
    data += "window_maximized=" + std::to_string(*maximized_window == true ? "true" : "false") + "\n";
    data += "window_fullscreen=" + std::to_string(*fullscreen == true ? "true" : "false") + "\n";
    return data;
}
