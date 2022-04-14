#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <stack>

template<class Component>
class ComponentArray
{
public:
    // not important
    ComponentArray() 
    {
        std::cout << "Array of \"" << typeid(Component).name() << "\" created" << std::endl;
    }
    
    void addComponentTo(Component c, uint id)
    {
        // check if entity already has a c component
        if (entityToComponent.count(id) != 0)
        {
            std::string componentType = typeid(Component).name();
            throw std::invalid_argument("Cannot add component <"+componentType+"> to entity #" + std::to_string(id) 
            + " because it already has one");
        }
        
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
            
            componentToEntity.push_back(0);
        }
        
        // indirections
        componentToEntity[index] = id;      // should mirror 'components' vector
        entityToComponent[id] = index;
    }
    
private:
    std::vector<Component> components;
    std::stack<uint> free_indexes;                      // used to recycle deleted components
    
    std::vector<uint>               componentToEntity;
    std::unordered_map<uint, uint>  entityToComponent;  // entity to component map

};