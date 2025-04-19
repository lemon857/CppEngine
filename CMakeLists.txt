cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME EngineTest)

set(ENGINE_PUBLIC_INCLUDES
	includes/EngineCore/Engine.h
	includes/EngineCore/Application.h
	includes/EngineCore/IComponent.h
	includes/EngineCore/IGameObject.h
	includes/EngineCore/IGame.h
	includes/EngineCore/Event.h
	includes/EngineCore/Keys.h
	includes/EngineCore/Input.h
	
	includes/EngineCore/UI/UImodule.h
	includes/EngineCore/UI/UIlayouts.h
		   
	includes/EngineCore/System/Log.h
	includes/EngineCore/System/Timer.h
	includes/EngineCore/System/TemplateTimer.h
	includes/EngineCore/System/Stopwatch.h
	includes/EngineCore/System/List.h
)

set(ENGINE_PRIVATE_INCLUDES
	includes/EngineCore/Window.h
	
	includes/EngineCore/Resources/ResourceManager.h
	includes/EngineCore/Resources/stb_image.h
	includes/EngineCore/Renderer/ShaderProgram.h
	includes/EngineCore/Renderer/Texture2D.h
	includes/EngineCore/Renderer/VertexBuffer.h
	includes/EngineCore/Renderer/IndexBuffer.h
	includes/EngineCore/Renderer/Renderer.h
	includes/EngineCore/Renderer/VertexArray.h
	includes/EngineCore/Renderer/VertexBufferLayout.h
	includes/EngineCore/Renderer/Point.h
	includes/EngineCore/Renderer/Line.h
	includes/EngineCore/Renderer/Camera.h
	includes/EngineCore/Renderer/Animator.h
	includes/EngineCore/Renderer/Animation.h
	includes/EngineCore/Renderer/Material.h
	includes/EngineCore/Renderer/ShaderProgramLayout.h
	
	includes/EngineCore/Physics/PhysicsEngine.h
	includes/EngineCore/Physics/Collider.h
	includes/EngineCore/Physics/MoveController.h		
	includes/EngineCore/Physics/Ray.h	
	
	includes/EngineCore/Renderer3D/GraphicsObject.h
	
	includes/EngineCore/System/ShadersSettings.h
	
	includes/EngineCore/Components/MeshRenderer.h	
	includes/EngineCore/Components/Transform.h
	includes/EngineCore/Components/SpriteRenderer.h
	includes/EngineCore/Components/Highlight.h
	
	includes/EngineCore/Meshes/Cube.h
	includes/EngineCore/Meshes/Sprite.h
	includes/EngineCore/Meshes/ObjModel.h
	includes/EngineCore/Meshes/EmptyObject.h
	includes/EngineCore/Meshes/Plane.h	
  
	includes/EngineCore/Light/DirectionalLight.h
	includes/EngineCore/Light/PointerLight.h
)

set(ENGINE_PRIVATE_SOURCES
	src/Engine/Engine.cpp
	src/Engine/Window.cpp
	src/Engine/Application.cpp
	src/Engine/Input.cpp
	
#	src/Examples/TanksDemo/TanksDemoGame.cpp
#	src/Examples/TanksDemo/TanksDemoGame.h
#	src/Examples/TanksDemo/Tank.cpp
#	src/Examples/TanksDemo/Tank.h
#	src/Examples/TanksDemo/BrickWall.h
#	src/Examples/TanksDemo/BrickWall.cpp
#	src/Examples/TanksDemo/Bullet.h
#	src/Examples/TanksDemo/Bullet.cpp
#	
#	src/Examples/SnakeDemo/SnakeDemoGame.cpp
#	src/Examples/SnakeDemo/SnakeDemoGame.h
#	src/Examples/SnakeDemo/SnakePart.h
#	src/Examples/SnakeDemo/SnakePart.cpp
#	src/Examples/SnakeDemo/SnakePart.h
#	
#	src/Examples/PongDemo/PongDemoGame.h
#	src/Examples/PongDemo/PongDemoGame.cpp
#	src/Examples/PongDemo/PongObject.h
#	src/Examples/PongDemo/PongObject.cpp
#	src/Examples/PongDemo/Ball.h
#	src/Examples/PongDemo/Ball.cpp
	
	src/Resources/ResourceManager.cpp
	
	src/Renderer/ShaderProgram.cpp
	src/Renderer/Texture2D.cpp
	src/Renderer/VertexBuffer.cpp
	src/Renderer/IndexBuffer.cpp
	src/Renderer/Renderer.cpp
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexBufferLayout.cpp
	src/Renderer/Point.cpp
	src/Renderer/Line.cpp
	src/Renderer/Camera.cpp	
	src/Renderer/Animator.cpp
	src/Renderer/Animation.cpp
	src/Renderer/Material.cpp
	src/Renderer/ShaderProgramLayout.cpp
	
	src/Physics/PhysicsEngine.cpp
	src/Physics/Collider.cpp
	src/Physics/MoveController.cpp
	src/Physics/Ray.cpp
	
	src/System/Timer.cpp
	src/System/Stopwatch.cpp
		
	src/UI/UImodule.cpp	
	src/UI/UIlayouts.cpp	
	
	src/Components/MeshRenderer.cpp	
	src/Components/Transform.cpp
	src/Components/SpriteRenderer.cpp
	src/Components/Highlight.cpp
	
	src/Meshes/Cube.cpp
	src/Meshes/Sprite.cpp
	src/Meshes/ObjModel.cpp
	src/Meshes/Plane.cpp
		
	src/Light/DirectionalLight.cpp
	src/Light/PointerLight.cpp
)


project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	${ENGINE_PRIVATE_SOURCES}
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/spdlog)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} spdlog)

target_include_directories(${PROJECT_NAME} PRIVATE includes)
include_directories(external/glm)
include_directories(external/rapidjson/include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
#Для игры копирование ресурсов из источника в билд
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

set(IMGUI_INCLUDES
	external/imgui/imgui.h
	external/imgui/backends/imgui_impl_opengl3.h
	external/imgui/backends/imgui_impl_glfw.h)
	
set(IMGUI_SOURCES
	external/imgui/imgui.cpp
	external/imgui/backends/imgui_impl_opengl3.cpp
	external/imgui/backends/imgui_impl_glfw.cpp
	external/imgui/imgui_widgets.cpp
	external/imgui/imgui_tables.cpp
	external/imgui/imgui_draw.cpp
	external/imgui/imgui_demo.cpp)

add_library(ImGUI STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES})
	
target_include_directories(ImGUI PUBLIC external)
target_include_directories(ImGUI PRIVATE external/imgui)

target_link_libraries(ImGUI glad glfw)

target_link_libraries(${PROJECT_NAME} ImGUI)