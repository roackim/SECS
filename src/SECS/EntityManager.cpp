// Includes
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

// Dependencies
#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager()
{
    Entity reserved(0);
    reserved.deleted = true;
    entities.push_back(reserved);   
}

uint EntityManager::createEntity()
{
    uint index;

    // if some deleted entities left some place, re-use it
    if (not free_indexes.empty())
    {
        index = free_indexes.top();         // get a recyclable Entity
        entities[index].deleted = false;    // declare entity as used
        free_indexes.pop();                 // remove index from stack
    }
    else
    {   
        index = entities.size();
        entities.push_back(Entity(index));
    }
    
    return index;
}

// only delete the Entity part
void EntityManager::deleteEntity(uint id)
{
    if (id >= entities.size())
    {
        throw std::invalid_argument("cannot delete entity #" + std::to_string(id));
    }
    if (entities[id].deleted == true)
    {
        throw std::invalid_argument("entity #" + std::to_string(id) + " already deleted");
    }

    // queue the index for recycling
    entities[id].deleted = true;
    free_indexes.push(id);
}

// get entity from (existing) id, throw if entity doesn't exists
const Entity& EntityManager::operator[](uint id)
{
    if (exists(id)) return entities[id];
    else throw std::invalid_argument("entity #" + std::to_string(id) + " doesn't exists");
}

Entity& EntityManager::get(uint id)
{
    if (exists(id)) return entities[id];
    else throw std::invalid_argument("entity #" + std::to_string(id) + " doesn't exists");
}

// check if entity exists
bool EntityManager::exists(uint id)
{
    
    if (id == 0) return false; // entity 0 is reserved (used in componentArray)
    if (id < entities.size())
    {
        return !entities[id].deleted;   
    }
    return false;
}

// only modifies the entity signature
void EntityManager::setComponentSignature(uint type, uint id)
{
    get(id).signature.set(type);
}

// only modifies the entity signature
void EntityManager::unsetComponentSignature(uint type, uint id)
{
    get(id).signature.reset(type);
}

bool EntityManager::has(uint type, uint id)
{
    return get(id).signature.test(type);   
}

std::vector<uint> EntityManager::filter(Signature s)
{
    std::vector<uint> filtered;
    
    for (Entity& e : entities)
    {
        if ((e.signature|s) == e.signature) // every component from s are already in e
        {
            filtered.push_back(e.id);
        }
    }
    return filtered;
}