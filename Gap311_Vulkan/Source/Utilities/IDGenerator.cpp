#include "IDGenerator.h"
#include <chrono>

int Brokkr::IDGenerator::GenerateUniqueID()
{
    static int counter = 0;
    return ++counter;
}

std::string Brokkr::IDGenerator::GenerateID(const std::string& name)
{
    return name + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}
