#pragma once
// ��������� ��� �������� ������� ��������

#include <string>
#include <map>

#include "IComponent.h"

class IGameObject
{
public:
	virtual void update(const double delta) { updateComponents(delta); };

	std::string get_name() { return m_name; };

	// ������� nullptr �������� ��� ������� ������������ ����������, ���� ��������� �������� ����������
	template <class _Ty>
	_Ty* addComponent()
	{
		if (getComponent<_Ty>() != nullptr) return nullptr;
		IComponent* component = (IComponent*)(new _Ty());
		component->set_target_object(this);
		m_components.emplace(typeid(_Ty).name(), component);
		return (_Ty*)component;
	};

	template <class _Ty>
	_Ty* getComponent()
	{
		componentsMap::const_iterator it = m_components.find(typeid(_Ty).name());
		if (it != m_components.end())
		{
			return (_Ty*)it->second;
		}
		return nullptr;
	};

protected:
	IGameObject(const std::string name)
		: m_name(name)
	{

	};
	~IGameObject()
	{
		m_components.clear();
	};

	void updateComponents(double delta)
	{
		for (auto curCom : m_components)
		{
			curCom.second->update(delta);
		}
	};

private:
	typedef std::map<std::string, IComponent*> componentsMap;
	componentsMap m_components;
	std::string m_name;
};