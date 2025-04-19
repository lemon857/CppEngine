#pragma once

#include <unordered_set>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include<glm/vec2.hpp>

class IGameObject;

namespace Physics
{
	class Collider;	

	enum EDirection
	{
		Up,
		Down,
		Left,
		Right
	};

	class PhysicsEngine
	{
	public:
		~PhysicsEngine() = delete;
		PhysicsEngine() = delete;

		PhysicsEngine(const PhysicsEngine&) = delete;
		PhysicsEngine(const PhysicsEngine&&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&&) = delete;

		// ������������� ����������� ������
		static void init(bool useGravity = false);
		static void addDynamicObj(std::shared_ptr<IGameObject> obj, int id);
		static void addImpulse(glm::vec2, int id);
		static void terminate();
		static void update(const double delta);
		// ���������� ��������� �� ������ � ����������� ���������� ������� ��������� ��� ����������� 
		// ������� ������� ��������� ������������ ������������ ����������
		static IGameObject* checkIntersection(std::shared_ptr<Collider> collider, EDirection outDir);
		static void addCollider(std::shared_ptr<Collider> collider);
	private:
		//static bool hasCollidersIntersection(const std::shared_ptr<Collider> collider1, const std::shared_ptr<Collider> collider2);

		typedef std::map<const int, std::shared_ptr<IGameObject>> objMap;
		static objMap m_dynamicObj;

		typedef std::map<const int, glm::vec2> ImpulseMap;
		static ImpulseMap m_impulses;

		static std::vector<std::shared_ptr<Collider>> m_colliders;

		static double m_g;
		static int m_currentID;

		static bool m_useGravity;
	};
}