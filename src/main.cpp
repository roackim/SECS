
#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/EntityManager.h"
#include "SECS/ComponentArray.hpp"
#include "SECS/ComponentManager.hpp"

int main(int argc, char* argv[])
{
    
    EntityManager em;
    
    
    uint e1 = em.createEntity();
    uint e2 = em.createEntity();
    uint e3 = em.createEntity();
    
    // ComponentArray<float> cf;
    // ComponentArray<std::string> cs;
    // cf.addComponentToEntity(3.14, e1);
    // cs.addComponentToEntity("yam", e1);
    
    ComponentManager cm;
    
    cm.addComponentToEntity(3.14, e1);
    cm.addComponentToEntity("yam", e1);
    cm.addComponentToEntity("may", e2);
    cm.addComponentToEntity("may", e3);
    
    return 0;   
}