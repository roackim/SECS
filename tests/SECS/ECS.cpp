#include "SECS/ECS.hpp"
#include "doctest.h"

TEST_CASE("ECS class Tests")
{
    ECS ecs;
    
    SUBCASE("Deleting Entity")
    {
        std::cout << "_________" << std::endl;
        
        uint e1 = ecs.newEntity();
        ecs.addComponent<char>('c', e1);
        ecs.addComponent<float>(3.14f, e1);
        
        auto& c1 = ecs.getComponent<char>(e1);
        auto& c2 = ecs.getComponent<float>(e1);
        
        CHECK(c1 == 'c');
        CHECK(c2 == 3.14f);
        
        ecs.deleteEntity(e1);
        
        // components do not exists anymore
        CHECK_THROWS(ecs.getComponent<char>(e1));
        CHECK_THROWS(ecs.getComponent<float>(e1));
        
        // entity doesn't exists anymore
        CHECK_THROWS(ecs.em[e1]);
    }
}