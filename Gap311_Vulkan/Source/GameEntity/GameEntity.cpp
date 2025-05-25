#include "GameEntity.h"

void Brokkr::GameEntity::Init() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Init();
    }
}

Brokkr::GameEntity::~GameEntity()
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Detach();
        m_pComponents[i]->Destroy();
    }
}

void Brokkr::GameEntity::Update() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Update();
    }
}


void Brokkr::GameEntity::Disable() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Disable();
    }
}

void Brokkr::GameEntity::Enable() const
{
    for (size_t i = 0; i < m_pComponents.size(); ++i)
    {
        m_pComponents[i]->Enable();
    }
}

