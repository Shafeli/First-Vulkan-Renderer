#pragma once
#include <string>
#include "../Component.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class CoreSystems;
    class EntityXMLParser;
    class RenderPipeline;
    class GameEntity;

    class RenderComponent final : public Component
    {
        GameEntity* m_pOwner = nullptr;

        // std::string m_meshFilePath;

        bool m_active;

    public:
        RenderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render() override;

        virtual void Attach() override;
        virtual void Detach() override;
        virtual void Enable() override;
        virtual void Disable() override;

        // Static registration function
        static void RegisterCreationFunction(EntityXMLParser* parser);
        static void CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element, CoreSystems* coreSystems);
    };
}
