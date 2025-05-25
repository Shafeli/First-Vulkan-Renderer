#include "RenderComponent.h"

#include <tinyxml2.h>

#include "../../GameEntity.h"
#include "../../../XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

Brokkr::RenderComponent::RenderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems)
{
}

bool Brokkr::RenderComponent::Init()
{
    return true; // defaulting this for now not sure there is a need for this one to Init anything
}

void Brokkr::RenderComponent::Update()
{

}

void Brokkr::RenderComponent::Destroy()
{
    //
}

void Brokkr::RenderComponent::Render()
{
    if (m_active)
    {

    }
}

void Brokkr::RenderComponent::Attach()
{
    //
}

void Brokkr::RenderComponent::Detach()
{
    //
}

void Brokkr::RenderComponent::Enable()
{
    Component::Enable();

}

void Brokkr::RenderComponent::Disable()
{
    Component::Disable();
    m_active = false;
}

void Brokkr::RenderComponent::RegisterCreationFunction(EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("RenderComponent", CreateComponent);
}

void Brokkr::RenderComponent::CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element,
    CoreSystems* coreSystems)
{

    entity->AddComponent<RenderComponent>(coreSystems);
}

