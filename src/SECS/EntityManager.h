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
    uint createEntity();
    void deleteEntity(uint id);
    const Entity& operator[](uint id);
    bool exists(uint id);
    void display();
    
// private: // attributes
    std::vector<Entity> entities;
    std::stack<uint> free_indexes;    // keep track of "deleted" entities -> reassign their index
};