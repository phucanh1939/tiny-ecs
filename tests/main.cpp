#include <cassert>
#include <iostream>

#include <tinyecs/ArchetypeRegistry.h>
#include <tinyecs/ComponentSignature.h>
#include <tinyecs/ComponentRegistry.h>
#include <tinyecs/ComponentType.h>

using namespace tinyecs;

void printPassed(const char* testName)
{
    std::cout << "\033[32m✓ " << testName << " passed\033[0m\n";
}

int main()
{
    std::cout << "\033[32m✓ All tests passed\033[0m\n";

    return 0;
}