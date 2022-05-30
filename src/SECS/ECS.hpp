#pragma once

#include <vector>
#include <iostream>
#include <bitset>

#include "ComponentManager.hpp"
#include "EntityManager.h"

namespace ecs 
{
    class ECS
    {
    public:
        EntityManager em;
        ComponentManager cm;
    };

    ECS ecs;
    
    template<class... Component>
    void constructSignature(Signature& s)
    {
        ((s.set(ecs.cm.getIndexFromType<Component>())), ...); // je comprend pas pourquoi ça marche ptdr
        // -> apparement ce serait appelé une "Primary Expression" en c++
    }
}

namespace ecs::entity
{
    uint create() { return ecs.em.createEntity(); }
    
    void destroy(uint id) 
    {
        const Entity& e = ecs.em[id];
        
        // delete all components from entity
        for (uint i=0; i<e.signature.size(); i++)
        {
            if (e.signature.test(i) == true)
            {
                ecs.cm.getComponentArrayPtr(i)->deleteComponent(id);
            }   
        }
        // delete the entity
        ecs.em.deleteEntity(id);
    }
    
    bool exists(uint id) { return ecs.em.exists(id); }
    
    template<class... Components>
    std::vector<uint> filter()
    {
        Signature s;
        constructSignature<Components...>(s);
        
        return ecs.em.filter(s);
    }
}

namespace ecs::component
{
    template<class Component>
    void add(Component c, uint id) 
    { 
        uint type = ecs.cm.addComponentToEntity(c, id); // get componentArray index
        ecs.em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void remove(Component c, uint id) 
    { 
        ecs.cm.deleteComponentFromEntity(c, id);
        uint type = ecs.cm.type_to_index(c);
        ecs.em.unsetComponentSignature(type, id);    
    }
    
    template<class Component>
    Component& get(uint id) 
    { 
        ComponentArray<Component>* ptr = ecs.cm.getComponentArrayPtr<Component>();
        return ptr->getComponent(id);
    }
}

namespace ecs::system
{
    
}