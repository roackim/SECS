#pragma once

// Includes
#include <iostream>
#include <bitset>
#include <cstdint>

struct Entity
{
    static const uint max_component = 64;
    
public:
    Entity(uint id_);
    
    // methods
    
    // attributes
    bool deleted = false;
    uint id;
    std::bitset<max_component> signature = 0;
};