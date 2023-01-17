#pragma once

#include <vector>
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
        static ECS& get_ecs()
        {
            static ECS ecs_instance;   
            return ecs_instance;   
        }
    
    private:
        ECS() {}
        ECS(ECS const&);                // Don't Implement.
        void operator=(ECS const&);     // Don't implement
    };

    
    template<class... Component>
    static void constructSignature(Signature& s)
    {
        ((s.set(ECS::get_ecs().cm.getIndexFromType<Component>())), ...); // je comprend pas pourquoi ça marche ptdr
        // -> apparement ce serait appelé une "Primary Expression" en c++
    }
}

namespace ecs::entity
{
    uint create();
    
    bool exists(uint id);
    
    void destroy(uint id);
    
    template<class... Components>
    bool has(uint id)
    {
        Signature s;
        constructSignature<Components...>(s);
        
        Signature& entity_s = ECS::get_ecs().em.get(id).signature;
        
        return ((entity_s|s) == entity_s); 
    }
    
    
    template<class... Components>
    std::vector<uint> filter()
    {
        Signature s;
        constructSignature<Components...>(s);
        
        return ECS::get_ecs().em.filter(s);
    }
    
}

namespace ecs::component
{
    template<class Component>
    void add(Component c, uint id) 
    {
        uint type = ECS::get_ecs().cm.addComponentToEntity(c, id); // get componentArray index
        ECS::get_ecs().em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void add(uint id)
    {
        Component c; // default initialization
        uint type = ECS::get_ecs().cm.addComponentToEntity(c, id); // get componentArray index
        ECS::get_ecs().em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void remove(uint id) 
    { 
        ECS::get_ecs().cm.deleteComponentFromEntity<Component>(id);
        uint type = ECS::get_ecs().cm.getIndexFromType<Component>();
        ECS::get_ecs().em.unsetComponentSignature(type, id);    
    }
    
    template<class Component>
    Component& get(uint id) 
    { 
        ComponentArray<Component>* ptr = ECS::get_ecs().cm.getComponentArrayPtr<Component>();
        return ptr->getComponent(id);
    }
}

namespace ecs::system
{
    
}