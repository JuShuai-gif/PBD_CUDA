#include <iostream>
#include "Actor.hpp"
#include "Component.hpp"

using namespace Velvet;

#define SET_COMPONENT_NAME name = __func__

void foo()
{
    const char* name;
    SET_COMPONENT_NAME;
    printf("Fun's Name is %s\n", name);

    printf("Leaving %s\n", name);
}

int main()
{
    Actor a("Test");
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    a.Initialize(v);
    std::cout << a.name << std::endl;
    
    return 0;
}