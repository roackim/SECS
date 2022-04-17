#include "SECS/ComponentManager.hpp"
#include "doctest.h"


TEST_CASE("ComponentManager Tests") // most of the cases are handled by ComponentArray
{                                   // this is mainly an interface to add component of any types
    ComponentManager cm;
    
    SUBCASE("Adding components: creating Component array")
    {
        ComponentArray<char>* ptr = cm.getComponentArrayPtr<char>();
        CHECK(ptr == nullptr);
        
        CHECK_NOTHROW(cm.addComponentToEntity<char>('d', 1));   // will instanciate a ComponnentArray<char>
        ptr = cm.getComponentArrayPtr<char>();
        CHECK(ptr != nullptr);
    }
    
    SUBCASE("Deleting components")
    {
        cm.addComponentToEntity<char>('d', 1);
        CHECK_NOTHROW(cm.deleteComponentFromEntity<char>(1));   // no throws
        CHECK_THROWS(cm.deleteComponentFromEntity<float>(1));   // throws: there is no instance of ComponentAray<float> yet
        cm.addComponentToEntity<float>(3.14f, 1);               // create an instance of ComponentArray<float> 
        CHECK_THROWS(cm.deleteComponentFromEntity<float>(3));  // ComponentArray<float> throws: entity #3 has no such component
    }
    
    SUBCASE("Component Signature")
    {
        uint sig1 = cm.addComponentToEntity<uint>(123, 1);   
        uint sig2 = cm.addComponentToEntity<uint>(456, 5);
        uint sig3 = cm.addComponentToEntity<char>('A', 3);
        
        CHECK(sig1 == sig2);
        CHECK(sig3 != sig1);
    }
}