#include "SECS/ECS.hpp"
#include "doctest.h"

TEST_CASE("ECS class Tests")
{
    SUBCASE("Deleting Entity")
    {
        uint e1 = ecs::entity::create();
        ecs::component::add<char>('c', e1);
        ecs::component::add<float>(3.14f, e1);
        
        auto& c1 = ecs::component::get<char>(e1);
        auto& c2 = ecs::component::get<float>(e1);
        
        CHECK(c1 == 'c');
        CHECK(c2 == 3.14f);
        
        ecs::entity::destroy(e1);
        
        // components do not exists anymore
        CHECK_THROWS(ecs::component::get<char>(e1));
        CHECK_THROWS(ecs::component::get<float>(e1));
        
        // entity doesn't exists anymore
        CHECK_THROWS(ecs::ecs.em[e1]);
    }
    
    SUBCASE("Filtering Entities")
    {
        uint e1 = ecs::entity::create();
        uint e2 = ecs::entity::create();
        
        ecs::component::add<char>('c', e1);
        ecs::component::add<float>(3.14f, e1);
        ecs::component::add<float>(3.14f, e2);
        
        auto ret1 = ecs::entity::filter<float, char>();
        auto ret2 = ecs::entity::filter<float>();
        
        CHECK(ret1.size() == 1);
        CHECK(ret2.size() == 2); 
    }
}