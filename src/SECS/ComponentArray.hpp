#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <stack>


// interface used in ComponentManager
class IComponentArray
{
public:
    IComponentArray() {}
    virtual ~IComponentArray() {}   // needs to be virtual so that derived destructor will also be called
                                    // if not, memory leak happens
                                    
    virtual void deleteComponent(uint id) = 0;
};


template<class Component>
class ComponentArray: public IComponentArray
{
public:
    // not important
    ComponentArray() 
    {
        std::cout << "Array of \"" << typeid(Component).name() << "\" created" << std::endl;
    }
    
    void addComponentToEntity(Component c, uint id)
    {
        // check if entity already has a c component
        if (entityToComponent.count(id) != 0)
        {
            std::string componentType = typeid(Component).name();
            throw std::invalid_argument("Cannot add component <"+componentType+"> to entity #" + std::to_string(id) 
            + " because it already has one");
        }
        if (id == 0) [[unlikely]]
            throw std::invalid_argument("Cannot add component to reserved entity #0");
        
        uint index;
        if (not free_indexes.empty())       // check if there are recyclable indexes
        {
            index = free_indexes.top();     // get a recyclable index
            components[index] = c;          // copy content of component to index
            free_indexes.pop();             // remove index from stack
        }
        else                                // otherwise expand vector if no recyclable indexes
        {
            index = components.size();
            components.push_back(c);
            componentToEntity.push_back(0); // make room, assigned right after
        }
        
        // keep indirections coherents
        componentToEntity[index] = id;      // should mirror 'components' vector
        entityToComponent[id] = index;
    }
    
    Component& getComponent(uint id)
    {
        if (id == 0) [[unlikely]] 
        {
            throw std::invalid_argument("Entity #0 is reserved and as such has no component");
        }
        
        auto itr = entityToComponent.find(id);
        if (itr == entityToComponent.end()) [[unlikely]]
        {
            throw std::invalid_argument("Entity #" + std::to_string(id) + " has no such component");
        }
        
        return components[itr->second];
    }
    
    void deleteComponent(uint id) override
    {
        
        auto itr = entityToComponent.find(id);
        if (itr == entityToComponent.end()) [[unlikely]]
        {
            throw std::invalid_argument("Entity #" + std::to_string(id) + " has no such component");
        }
        if (componentToEntity[itr->second] == 0) [[unlikely]]
        {
            throw std::invalid_argument("Entity #" + std::to_string(id) + " has no such component");
        }
        
        uint index = itr->second;
        componentToEntity[index] = 0;   // remove the component -> entity relation
        free_indexes.push(index);       // schedule index for recycling
        entityToComponent.erase(id);    // remove the entity -> component relation
        
        std::cout << "DELETING COMPONENT FOR #" << id << std::endl;
        
    }
    
    Component& operator[](uint id)
    {
        return getComponent(id);
    }
    
    bool exists(uint id)
    {
        auto itr = entityToComponent.find(id);
        if (itr == entityToComponent.end()) return false;
        
        return (componentToEntity[itr->second] != 0);
    }
    
// private:
    std::vector<Component> components;
    std::stack<uint> free_indexes;          // used to lookup inactive components
    
    std::vector<uint>               componentToEntity;
    std::unordered_map<uint, uint>  entityToComponent;  // entity to component map

};