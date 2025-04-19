#pragma once
// ��������� ��� �������� ������� ��������

#include <string>
#include <map>
#include <typeinfo>

#include "IComponent.h"

class IGameObject
{
public:
	virtual void update(const double delta) { updateComponents(delta); };

	std::string get_name() { return m_name; };

	void set_name(const std::string name) { m_name = name; };

	// ������� nullptr �������� ��� ������� ������������ ����������, ���� ��������� �������� ����������
	template <class _Ty, class... _Types>
	_Ty* addComponent(_Types&&... _Args)
	{
		if (getComponent<_Ty>() != nullptr) return nullptr;
		IComponent* component = (IComponent*)(new _Ty(std::forward<_Types>(_Args)...));
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

	template <class _Ty>
	void deleteComponent()
	{
		for (auto pos = m_components.begin(); pos != m_components.end();)
		{
			if (pos->first == typeid(_Ty).name())
			{
				delete pos->second;
				pos = m_components.erase(pos);
			}
			else
			{
				++pos;
			}
		}
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