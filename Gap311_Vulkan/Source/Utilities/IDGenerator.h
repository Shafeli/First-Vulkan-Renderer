#pragma once
#include <string>


/////////////////////////////////////////////////
//          ID Generator
//
//  Example Use:
//
// class Example
// {
//
//    std::string m_id;
//
// public:
//    Component(const std::string& name)
//        : m_id(IDGenerator::GenerateID(name))
//    { }
//
//    const std::string& GetID() const { return m_id; }
//
// };
//
/////////////////////////////////////////////////

namespace Brokkr
{
    class IDGenerator
    {
    public:
        static int GenerateUniqueID();
        static std::string GenerateID(const std::string& name);
    };
}
