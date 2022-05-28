#pragma once

// Includes
#include <unordered_map>
#include <vector>
#include <stack>
#include <bitset>

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
    
    void setComponentSignature(uint type, uint id);  
    void unsetComponentSignature(uint type, uint id);
    bool has(uint type, uint id);
    
    std::vector<uint> filter(Signature s);
    
    
private: // attributes
    std::vector<Entity> entities;
    std::stack<uint> free_indexes;    // keep track of "deleted" entities -> reassign their index
};