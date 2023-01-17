#pragma once

// Includes
#include <iostream>
#include <bitset>
// #include <cstdint>

typedef std::bitset<64> Signature; // 64 -> maximum number of different components

struct Entity
{
    
public:
    Entity(uint id_);
    
    // methods
    
    // attributes
    bool deleted = false;
    uint id;
    Signature signature = 0;
};