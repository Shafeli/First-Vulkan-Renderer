#pragma once

#include <functional>
#include <utility>

#include "../../XMLManager.h"
namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class GameEntity;
    class CoreSystems;
    class GameEntityManager;
    

    class EntityXMLParser final : public XMLParser
    {
        /*struct PreFab
        {
            // just in case need more stuff for later....
            GameEntity* m_pPrefab;
        };*/

        std::unordered_map<std::string, std::function<void(GameEntity*, tinyxml2::XMLElement*, CoreSystems*)>> m_componentConstructors;

    public:

        explicit EntityXMLParser(CoreSystems* pCoreSystems);

        // Function for components to register their creation functions
        void RegisterComponentCreationFunction
        (
            const std::string& componentName,
            std::function<void(GameEntity*, tinyxml2::XMLElement*, CoreSystems*)> creationFunction
        )
        {
            m_componentConstructors[componentName] = std::move(creationFunction);
        }

        virtual bool Parse(tinyxml2::XMLDocument& doc) override;

        void BuildEntity(GameEntity* pEntity, const std::string& entityName, const char* fileName);
    };
}
