#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "../CoreSystems/CoreSystems.h"

namespace tinyxml2
{
    class XMLDocument;
}

namespace Brokkr
{
    class CoreSystems;

    class XMLParser 
    {
    public:
        XMLParser(CoreSystems* pCoreSystems)
            :m_pCoreSystems(pCoreSystems)
        {
            //
        }
        virtual ~XMLParser() = default;
        virtual bool Parse(tinyxml2::XMLDocument& doc) = 0;

    protected:
        CoreSystems* m_pCoreSystems = nullptr;

    };

    class XMLManager final : public CoreSubsystem
    {
        
        CoreSystems* m_pCoreSystems = nullptr;
        std::vector<std::unique_ptr<XMLParser>> m_parsers;
        std::unordered_map<std::string, const char*> m_xmlPaths;

    public:

        explicit XMLManager(CoreSystems* assetManager) : CoreSubsystem(assetManager) {}
        void Init(CoreSystems* coreSystems);
        void Load(const char* filePath);
        const char* Get(const std::string& fileName);

        bool Parse(const char* fileName) const; // TODO: Remove this method of "parsing" 

        virtual void Destroy() override;

        template <typename ParserType, typename ... Args>
        ParserType* AddParser(Args&&... args)
        {
            // Create a instance of the Parser Type
            std::unique_ptr<ParserType> newParserType = std::make_unique<ParserType>(std::forward<Args>(args)...);

            // Get a raw pointer to the parser
            ParserType* result = newParserType.get();

            // Add the parser to the vector
            m_parsers.emplace_back(std::move(newParserType));

            // Return a pointer
            return result;
        }

        template <typename ParserType>
        ParserType* GetParser()
        {
            // Iterate through all components in the vector
            for (auto& parser : m_parsers)
            {
                // If the cast is successful
                if (ParserType* target = dynamic_cast<ParserType*>(parser.get()))
                {
                    return target;
                }
            }

            // If no type is found
            return nullptr;
        }
    };
}
