
#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/ECS.hpp"

int main(int argc, char* argv[])
{
    
    uint e1 = ecs::entity::create();
    uint e2 = ecs::entity::create();
    uint e3 = ecs::entity::create();
    
    float f = 3.14;
    uint i = 404;
    
    ecs::component::add(f, e1);
    ecs::component::add(i, e2);    
        
    return 0;
    
}