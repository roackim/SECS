#pragma once

#include <vector>
#include <iostream>

#include "ComponentManager.hpp"
#include "EntityManager.h"

class ECS
{
public:
    EntityManager em;
    ComponentManager cm;
    
    uint newEntity() { return em.createEntity(); }
    void deleteEntity(uint id) { em.deleteEntity(id); }
    bool exists(uint id) { return em.exists(id); }
    
    template<class Component>
    void addComponent(Component c, uint id) 
    { 
        cm.addComponentToEntity(c, id); 
    }
    
    template<class Component>
    void deleteComponent(Component c, uint id) { cm.deleteComponentFromEntity(c, id); }
    
    template<class Component>
    Component& getComponent(uint id) 
    { 
        ComponentArray<Component>* ptr = cm.getComponentArrayPtr<Component>();
        return ptr->getComponent(id);
    }
};
