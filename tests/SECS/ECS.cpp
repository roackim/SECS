#include "SECS/ECS.hpp"
#include "doctest.h"

TEST_CASE("ECS class Tests")
{
    ECS ecs;
    
    SUBCASE("Removing Entities")
    {
        uint e1 = ecs.newEntity();
        uint e2 = ecs.newEntity();
        
        ecs.addComponent<float>(3.14f, e1);
        ecs.addComponent<float>(5.23f, e2);
        ecs.addComponent<uint>(17, e2);
        
        CHECK(ecs.getComponent<float>(e1) == 3.14f);
        CHECK(ecs.getComponent<uint>(e2) == 17);
        
        CHECK_THROWS(ecs.getComponent<double>(e2));
        
        // deleting an entity should not have any consequence on others
        ecs.deleteEntity(e1);
        CHECK(ecs.getComponent<float>(e2) == 5.23f);
    }
}