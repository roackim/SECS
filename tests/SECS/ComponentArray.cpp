#include "SECS/ComponentArray.hpp"
#include "doctest.h"


TEST_CASE("ComponentArray Tests") 
{
    ComponentArray<char> ca;
    SUBCASE("Adding components")
    {
        CHECK_THROWS(ca.addComponentToEntity('c', 0));           // entity 0 is reserved
        ca.addComponentToEntity('h', 1);                    
        CHECK_THROWS(ca.addComponentToEntity('a', 1));   // cannot add 2 of the same component to the same entity
    }
    
    SUBCASE("Accessing components")
    {
        // accessing wrongly
        CHECK_THROWS(ca.getComponent(0));   // reserved entity
        CHECK_THROWS(ca.getComponent(1));   // non entity without such a component registered
        
        // accessing and modifying
        ca.addComponentToEntity('$', 3);    
        ca.getComponent(3) = '#';
        CHECK(ca[3] == '#');
        ca[3] = '%';                        // [] operator calls getComponent, they are the same
        CHECK(ca[3] == '%');
        
        CHECK(ca.exists(3) == true);  
    }
    
    SUBCASE("Deleting components")
    {
        CHECK_THROWS(ca.deleteComponent(0));    // questionnable test
        CHECK_THROWS(ca.deleteComponent(1));    // entity 1 has no component registered yet
        
        CHECK(ca.exists(1) == false);           
        ca.addComponentToEntity('A', 1);
        CHECK(ca.exists(1) == true);
        
        char& ref1 = ca[1];                     // store address of component
        
        CHECK_NOTHROW(ca.deleteComponent(1));   // can now delete component of entity 1
        CHECK(ca.exists(1) == false);           // deletion is done
        
        ca.addComponentToEntity('B', 2);        // this component should take the place of the previously deleted one, through recycling
        CHECK(ca.exists(2) == true);
        CHECK(ca[2] == 'B');
        
        CHECK(&ref1 == &ca[2]);                 // check that recycling took place
    }
}