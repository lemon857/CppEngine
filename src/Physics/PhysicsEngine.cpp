#include "PhysicsEngine.h"

#include "../Engine/IGameObjec.h"
#include "Collider.h"

namespace Physics
{
	int PhysicsEngine::m_currentID = 0;
	double PhysicsEngine::m_g;
	bool PhysicsEngine::m_useGravity;
	PhysicsEngine::objMap PhysicsEngine::m_dynamicObj;
	PhysicsEngine::ImpulseMap PhysicsEngine::m_impulses;
	std::vector<std::shared_ptr<Collider>> PhysicsEngine::m_colliders;

	void PhysicsEngine::init(bool useGravity)
	{
		m_useGravity = useGravity;
		m_g = 9.8;
	}
	void PhysicsEngine::terminate()
	{

	}
	void PhysicsEngine::update(const double delta)
	{

	}
	void PhysicsEngine::addDynamicObj(std::shared_ptr<IGameObject>& obj, int id)
	{
		m_dynamicObj.emplace(id, std::move(obj));
	}
	void PhysicsEngine::addImpulse(glm::vec2 impulse, int id)
	{
		m_impulses.emplace(id, impulse);
	}
	// �� ����������� ���� �������� ������� (������� �� ����)
	bool PhysicsEngine::checkIntersection(std::shared_ptr<Collider>& collider, std::string& outName, EDirection& outDir)
	{
		for (auto currentCollider : m_colliders)
		{
			glm::vec2 pos = collider->getPosition();
			glm::vec2 curPos = currentCollider->getPosition();
			glm::vec2 size = collider->getSize();
			glm::vec2 curSize = currentCollider->getSize();
			
			if (curPos.y > pos.y + size.y || curPos.y + curSize.y < pos.y) continue;

			if (curPos.x + curSize.x < pos.x || curPos.x > pos.x + size.x) continue;

			if (pos == curPos && size == curSize) continue;

			outName = currentCollider->getObject().getName();
			return true;
		}
		return false;
	}
	void PhysicsEngine::addCollider(std::shared_ptr<Collider>& collider)
	{
		m_colliders.push_back(collider);
	}
	/*bool PhysicsEngine::hasCollidersIntersection(const std::shared_ptr<Collider> collider1,	const std::shared_ptr<Collider> collider2)
	{
		const glm::vec2 collider1_bottomLeft_world = collider1->m_object->getPosition();
		const glm::vec2 collider1_topRight_world = collider1->m_object->getPosition() + collider1->m_object->getSize();

		const glm::vec2 collider2_bottomLeft_world = collider2->m_object->getPosition();
		const glm::vec2 collider2_topRight_world = collider2->m_object->getPosition() + collider1->m_object->getSize();

		if (collider1_bottomLeft_world.x >= collider2_topRight_world.x)
		{
			return false;
		}
		if (collider1_topRight_world.x <= collider2_bottomLeft_world.x)
		{
			return false;
		}

		if (collider1_bottomLeft_world.y >= collider2_topRight_world.y)
		{
			return false;
		}
		if (collider1_topRight_world.y <= collider2_bottomLeft_world.y)
		{
			return false;
		}

		return true;
	}*/
}