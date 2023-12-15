#include "Games/Tower_Defense/BaseEnemy.h"

#include "Games/Tower_Defense/Castle.h"
#include "Games/Tower_Defense/BaseEffect.h"
#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#define HALF_PLANE 30.f

BaseEnemy::BaseEnemy(ObjModel* model, Castle* target, glm::vec3 pos, double cooldown, double velocity, const unsigned int hp,
	std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 color)
	: m_cool_down(cooldown * 1000)
	, m_target_castle(std::move(target))
	, m_cur_time(0)
	, m_velocity(velocity)
	, m_model(std::move(model))
	, m_bar(new HealthBar(pMaterial, pos + glm::vec3(0.f, 2.5f, 0.f), 25, 2, hp, glm::vec3(1.f), color))
	, m_isDestroyed(false)
	, m_hp(hp)
	, m_effect(nullptr)
{
	m_model->addComponent<Transform>(pos, glm::vec3(1.f));
}

BaseEnemy::~BaseEnemy()
{
	delete m_bar;
	delete m_effect;
}

void BaseEnemy::update(const double delta)
{
	if (m_isDestroyed) return;
	glm::vec3 a = m_target_castle->get_pos() - m_model->getComponent<Transform>()->get_position();
	if (sqrt(a.x * a.x + a.z * a.z) > MIN_DISTANCE_TO_CASTLE)
	{
		m_model->getComponent<Transform>()->add_position(glm::normalize(a) * (float)delta * (float)m_velocity);
		if (m_model->getComponent<Transform>()->get_position().x > HALF_PLANE)
		{
			m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
		}
		else
		{
			m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, -1 * glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
		}
		m_bar->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.5f, 0.f));
	}
	else
	{
		if (m_cur_time < m_cool_down) m_cur_time += delta;
		else
		{
			m_target_castle->damage(5);
			m_cur_time = 0;
		}
	}
	if (m_effect != nullptr)
	{
		if (m_effect->is_destroy()) delete m_effect;
		else m_effect->update(delta);
	}
}

void BaseEnemy::render()
{
	if (m_isDestroyed) return;
	m_model->update(0);
	m_bar->update();
}

void BaseEnemy::damage(const unsigned int damage_hp)
{
	m_hp -= damage_hp;
	if (m_hp <= 0)
	{
		m_isDestroyed = true;
	}
	m_bar->set_value(m_hp);
}

glm::vec3 BaseEnemy::get_pos()
{
	return m_model->getComponent<Transform>()->get_position();
}

void BaseEnemy::set_effect(BaseEffect* effect)
{
	if (m_effect != nullptr) delete m_effect;
	m_effect = std::move(effect);
	m_effect->set_cur_enemy(this);
}
