#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>

#include "ComponentArray.hpp"
#include "Entity.h"

class ComponentManager
{
public:
    template<class Component>
    uint addComponentToEntity(Component c, uint id)     // assumes entity exists
    {                                                   // returns component index (used for signature)
        auto t = std::type_index(typeid(Component));
        if (type_to_index.count(t) == 0)                // if componentArray doesn't exists create it
        {
            uint index = component_arrays.size();
            IComponentArray* ptr = new ComponentArray<Component>();
            component_arrays.push_back(ptr);
            type_to_index[t] = index;
        }
        
        auto* ptr = getComponentArrayPtr<Component>();
        ptr->addComponentToEntity(c, id);
        
        return type_to_index[t];
    }
    
    template<class Component>
    ComponentArray<Component>* getComponentArrayPtr() 
    {
        auto t = std::type_index(typeid(Component));
        
        if (type_to_index.count(t) == 0) [[unlikely]]
        {
            throw std::invalid_argument("No ComponnentArray of this type has been instanced yet.");
        }
        uint i = type_to_index[t];
        
        return static_cast<ComponentArray<Component>*>(component_arrays[i]);
    }
    
    IComponentArray* getComponentArrayPtr(uint index) 
    {
        if (component_arrays.size() <= index) [[unlikely]]
        {
             throw std::invalid_argument("No ComponnentArray of this type has been instanced yet.");
        }
        return component_arrays[index];
    }
    
    template<class Component>
    void deleteComponentFromEntity(uint id)
    {
        auto* ptr = getComponentArrayPtr<Component>();
        if (ptr == nullptr) [[unlikely]] throw std::invalid_argument("No ComponnentArray of this type has been instanced yet.");
        
        ptr->deleteComponent(id);
    }
    
    template<class Component>
    uint getIndexFromType()
    {
        auto t = std::type_index(typeid(Component)); // get value from type
        
        if (type_to_index.count(t) == 0) [[unlikely]] // check that it should exists
        {
            throw std::invalid_argument("No ComponnentArray of this type has been instanced yet.");
        }
        
        return type_to_index[t];   
    }
    
    ~ComponentManager() // free allocated ComponentArrays
    {
        for (IComponentArray* ptr : component_arrays)
        {
            delete ptr;   
        }   
    }
    
private:
    // attributes
    std::vector<IComponentArray*> component_arrays;
    std::unordered_map<std::type_index, uint> type_to_index;
};