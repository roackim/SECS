#pragma once

#include <unordered_map>
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
    virtual void deleteComponent(uint id) {}
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
        
        // keep indirections coherents
        componentToEntity[index] = id;      // should mirror 'components' vector
        entityToComponent[id] = index;
    }
    
private:
    std::vector<Component> components;
    std::vector<bool> component_state;      // 1: active, 0: scheduled for recycling
    std::stack<uint> free_indexes;          // used to lookup inactive components
    
    std::vector<uint>               componentToEntity;
    std::unordered_map<uint, uint>  entityToComponent;  // entity to component map

};