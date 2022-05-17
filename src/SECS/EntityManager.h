#pragma once

// Includes
#include <unordered_map>
#include <vector>
#include <stack>

// Dependencies
#include "Entity.h"

class EntityManager
{
public: // methods
    EntityManager();
    uint createEntity();
    void deleteEntity(uint id);
    const Entity& operator[](uint id);
    Entity& get(uint id);
    
    bool exists(uint id);
    
    void addComponent(uint type, uint id);  
    void deleteComponent(uint type, uint id);
    bool has(uint type, uint id);
    
    
private: // attributes
    std::vector<Entity> entities;
    std::stack<uint> free_indexes;    // keep track of "deleted" entities -> reassign their index
};