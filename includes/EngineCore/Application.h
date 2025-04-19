#pragma	once
#include "EngineCore/Event.h"
#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Physics/Ray.h"
#include "EngineCore/Meshes/Cube.h"
#include "EngineCore/Meshes/Sprite.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/System/List.h"

#include <memory>
#include <glm/vec2.hpp>
#include <list>

class Application
{
public:
	Application();
	virtual  ~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	int start(glm::ivec2& window_size, const char* title);

	virtual bool init() { return true; };

	bool _init();

	bool init_events();

	void on_key_update(const double delta);

	void on_mouse_button_event(const MouseButton button, const double pos_x, const double pos_y, const bool isPressed);

	virtual void on_ui_render() {};
	// TEMP
	template <class _Ty, class... _Types>
	void add_object(_Types&&... _Args)
	{
		IGameObject* obj = (IGameObject*)(new _Ty(std::forward<_Types>(_Args)...));
		m_items_str.push_back(obj->get_name());
		m_objs.push_back(std::move(obj));
	}
protected:
	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;
	bool m_maximized_window = false;
	glm::ivec2 m_window_position = glm::ivec2(100);

	Camera* m_cam;

	Ray* m_ray;

	RenderEngine::Line* m_line;

	RenderEngine::Line* m_line_transform;

	float m_colors[4] = { 0.33f, 0.33f, 0.33f, 0.f };
	float m_cam_pos[3] = { -5.f, 0.f, 0.f };
	float m_cam_rot[3] = { 0.f, 0.f, 0.f };

	float m_light_color[3] = { 1.f, 1.f, 1.f };

	int item_current = -1;

	float m_cam_velocity = 0.01f;
	float m_cam_rotate_velocity = 0.1f;
	float m_cam_sensetivity = 0.5f;
	float m_cam_fov = 60.f;
	float m_cam_sov = 1.f;
	float m_cam_far_plane = 100.f;

	float m_add_ctrl_speed = 2.f;

	float m_ambient_factor = 0.1f;
	float m_diffuse_factor = 1.f;
	float m_specular_factor = 1.f;
	float m_diffuse_specular_factor = 0.5f;
	float m_metalic_factor = 0.f;
	float m_shininess = 1.f;

	float m_distance = 10.f;

	double m_init_mouse_pos_x = 0;
	double m_init_mouse_pos_y = 0;

	int m_mouse_pos_x = 0;
	int m_mouse_pos_y = 0;
	int m_fps = 0;

	double m_world_mouse_dir_x = 0;
	double m_world_mouse_dir_y = 0;
	double m_world_mouse_dir_z = 0;
	
	bool m_isPerspectiveCam = true;
	bool m_isInversiveMouseY = false;
	bool m_drawNullIntersection = false;
	bool m_moveObject = false;
	bool m_isUIhovered = false;

	std::vector<std::string> m_items_str;

	linked_list<IGameObject*> m_objs;
};