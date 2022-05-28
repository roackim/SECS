#pragma once

#include <vector>
#include <iostream>
#include <bitset>

#include "ComponentManager.hpp"
#include "EntityManager.h"

class ECS
{
public:
    EntityManager em;
    ComponentManager cm;
    
    uint newEntity() { return em.createEntity(); }
    
    void deleteEntity(uint id) 
    {
        const Entity& e = em[id];
        
        // delete all components from entity
        for (uint i=0; i<e.signature.size(); i++)
        {
            if (e.signature.test(i) == true)
            {
                cm.getComponentArrayPtr(i)->deleteComponent(id);
            }   
        }
        // delete the entity
        em.deleteEntity(id);
    }
    
    bool exists(uint id) { return em.exists(id); }
    
    template<class Component>
    void addComponent(Component c, uint id) 
    { 
        uint type = cm.addComponentToEntity(c, id); // get componentArray index
        em.setComponentSignature(type, id);
    }
    
    template<class Component>
    void deleteComponent(Component c, uint id) 
    { 
        cm.deleteComponentFromEntity(c, id);
        uint type = cm.type_to_index(c);
        em.unsetComponentSignature(type, id);    
    }
    
    template<class Component>
    Component& getComponent(uint id) 
    { 
        ComponentArray<Component>* ptr = cm.getComponentArrayPtr<Component>();
        return ptr->getComponent(id);
    }
    
    template<class... Components>
    std::vector<Entity*> filterEntities()
    {
        Signature s;
        constructSignature<Components...>(s);
        
        return em.filter(s);
    }
    
private:

    template<class... Component>
    void constructSignature(Signature& s)
    {
        ((s.set(cm.getIndexFromType<Component>())), ...);
    }

};
