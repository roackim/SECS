#include "ECS.hpp"


// ecs::ECS& ecs::ECS::get_ecs()
// {
//     static ECS ecs_instance;   
//     return ecs_instance;
// }

uint ecs::entity::create() 
{
    return ECS::get_ecs().em.createEntity(); 
}

bool ecs::entity::exists(uint id) { return ECS::get_ecs().em.exists(id); }

void ecs::entity::destroy(uint id) 
{
    const Entity& e = ECS::get_ecs().em[id];
    
    // delete all components from entity
    for (uint i=0; i<e.signature.size(); i++)
    {
        if (e.signature.test(i) == true)
        {
            ECS::get_ecs().cm.getComponentArrayPtr(i)->deleteComponent(id);
        }   
    }
    // delete the entity
    ECS::get_ecs().em.deleteEntity(id);
}
