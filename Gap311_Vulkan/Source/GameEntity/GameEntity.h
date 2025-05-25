#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../Utilities/IDGenerator.h"
#include "Component/Component.h"


#include "Component/RenderComponent/RenderComponent.h"
#include <tinyxml2.h>


namespace Brokkr
{

    class GameEntity
    {
        int m_id;
        std::vector<std::unique_ptr<Component>> m_pComponents;
    public:

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        GameEntity() : m_id(IDGenerator::GenerateUniqueID()){}

        GameEntity(const GameEntity& other) = delete;
        GameEntity& operator=(const GameEntity& other) = delete;
        GameEntity(GameEntity&& other) noexcept = default;
        GameEntity& operator=(GameEntity&& other) noexcept = default;

        ~GameEntity();

        void Init() const;

        [[nodiscard]] int GetId() const { return m_id; }

        // Object Update Components
        ///////////////////////////////////////////
        void Update() const;

        // Object Disable
        ///////////////////////////////////////////
        void Disable() const;

        // Object Enable
        ///////////////////////////////////////////
        void Enable() const;

        // Object Get a Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        ComponentType* GetComponent();

        // Object Attach Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallAttachOnComponent();

        // Object Detach Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallDetachOnComponent();
         
        // Object Enable Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallEnableOnComponent();

        // Object Disable Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallDisableOnComponent();
        template <class ComponentType, class ... Args>
        ComponentType* AddComponent(const std::string& xmlString, Args&& ... args);

        // Object Add a Component
        ///////////////////////////////////////////
        template<typename ComponentType, typename... Args>
        ComponentType* AddComponent(Args&&... args);

    };

    template <typename ComponentType>
    ComponentType* GameEntity::GetComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                return target;
            }
        }

        // If no component of type is found
        return nullptr;
    }

    template <typename ComponentType>
    void GameEntity::CallAttachOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Attach();
            }
        }
    }

    template <typename ComponentType>
    void GameEntity::CallDetachOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Detach();
            }
        }
    }

    template<typename ComponentType>
    inline void GameEntity::CallEnableOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Enable();
            }
        }
    }

    template<typename ComponentType>
    inline void GameEntity::CallDisableOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Disable();
            }
        }
    }

    template <typename ComponentType, typename... Args>
    inline ComponentType* GameEntity::AddComponent(const std::string& xmlString, Args&&... args)
    {
        // Create an instance of the component type passing the current GameEntity pointer
        std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);

        // Parse the XML string to init the component
        newComponent->ParseFromXML(xmlString);

        // Get a raw pointer to the component
        ComponentType* result = newComponent.get();

        // Add the component to the vector
        m_pComponents.emplace_back(std::move(newComponent));

        // Return a pointer
        return result;
    }

    template <typename ComponentType, typename ... Args>
    ComponentType* GameEntity::AddComponent(Args&&... args)
    {
        // Create a instance of the component type passing in the current GameEntity pointer
        std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);

        // Get a raw pointer to the component
        ComponentType* result = newComponent.get();

        // Add the component to the vector
        m_pComponents.emplace_back(std::move(newComponent));

        // Return a pointer
        return result;
    }
}
