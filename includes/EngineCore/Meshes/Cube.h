#pragma once

#include "EngineCore/IGameObject.h"

#include <string>

namespace RenderEngine
{
	class Material;
}
// ������� ����� ��� ������������� 3� ����
class Cube : public IGameObject
{
public:
	Cube(RenderEngine::Material* pMaterial);

    ~Cube() = default;

    Cube(Cube&) = delete;
    Cube(Cube&&) = delete;

    Cube& operator=(Cube&) = delete;
    Cube& operator=(Cube&&) = delete;
private:
    RenderEngine::Material* m_pMaterial;
};