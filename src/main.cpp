
#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <stack>

#include "SECS/EntityManager.h"


int main()
{
    EntityManager em;
    uint e1 = em.createEntity();
    uint e2 = em.createEntity();
    uint e3 = em.createEntity();
    
    std::cout << "size: " << em.entities.size() << std::endl;
    em.deleteEntity(e2);
    std::cout << "size: " << em.entities.size() << std::endl;
    
    const Entity& e4 = em.createEntity();
    
    std::cout << "size: " << em.entities.size() << std::endl;
    std::cout << em[e2].deleted << std::endl;
    
    
    
    
    return 0;   
}