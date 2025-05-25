#include "EntityXMLParser.h"
#include <tinyxml2.h>
#include "../../../GameEntity/GameEntity.h"

Brokkr::EntityXMLParser::EntityXMLParser(CoreSystems* pCoreSystems): XMLParser(pCoreSystems)
{
    // Building functions
    RenderComponent::RegisterCreationFunction(this);

}

bool Brokkr::EntityXMLParser::Parse(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Entity" element
    if (!root || strcmp(root->Name(), "Entity") != 0)
    {
        // The XML file is not a valid Entity file
        return false;
    }

    return true;
}

void Brokkr::EntityXMLParser::BuildEntity(GameEntity* pEntity, const std::string& entityName, const char* fileName)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName) != tinyxml2::XML_SUCCESS)
    {
        // Error handling
        return;
    }

    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Entity" element
    if (!root || std::string(root->Name()) != "Entity")
    {
        // The XML file is not a valid Entity file
        return;
    }

    // Loop through the entities
    for (tinyxml2::XMLElement* entityElement = root->FirstChildElement("GameEntity");
        entityElement != nullptr;
        entityElement = entityElement->NextSiblingElement("GameEntity"))
    {
        const char* nameAttr = entityElement->Attribute("name");
        if (std::string(nameAttr) == entityName)
        {
            // Loop through the components of the element
            for (tinyxml2::XMLElement* pComponentElement = entityElement->FirstChildElement();
                pComponentElement != nullptr;
                pComponentElement = pComponentElement->NextSiblingElement())
            {
                const std::string componentName = pComponentElement->Name();
                auto iterator = m_componentConstructors.find(componentName);
                if (iterator != m_componentConstructors.end())
                {
                    iterator->second(pEntity, pComponentElement, m_pCoreSystems);
                }
            }
            return;
        }
    }
}