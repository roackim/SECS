
#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/EntityManager.h"
#include "SECS/ComponentArray.hpp"

int main()
{
    EntityManager em;
    uint e1 = em.createEntity();
    uint e2 = em.createEntity();
    uint e3 = em.createEntity();
    
    ComponentArray<float> cf;
    cf.addComponentTo(3.14, e1);
    
    return 0;   
}