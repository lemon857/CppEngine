#pragma once

#include <memory>
#include <functional>
#include<glm/vec2.hpp>
#include "EngineCore/IGameObject.h"
#include "EngineCore/IComponent.h"
#include "EngineCore/Physics/PhysicsEngine.h"


namespace Physics 
{
	using onCollisionCallback = std::function<void(IGameObject&, IGameObject&, Physics::EDirection)>;
	class Collider : public IComponent
	{
	public:
		Collider(IGameObject& targetObj)
			: IComponent(targetObj)
		{};

		void update(const double delta) override;
		// ������� ������� ������� ������� ��� ������� � �������� � �� ��� �������
		void setOnCollisionCallback(onCollisionCallback callback);

		glm::vec2& getSize() { return m_targetObj.getSize(); };
		glm::vec2& getPosition() { return m_targetObj.getPosition(); };
		IGameObject& getObject() { return m_targetObj; }
	private:
		onCollisionCallback m_onCollisionCallback;
	};
}