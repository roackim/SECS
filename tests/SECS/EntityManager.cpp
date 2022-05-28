#include "SECS/EntityManager.h"
#include "doctest.h"

TEST_CASE("EntityManager Tests") {
    EntityManager em;
    
    SUBCASE("Deleting entities")
    {
        CHECK_THROWS(em.deleteEntity(0));   // entity 0 is reserved
        CHECK_THROWS(em.deleteEntity(11));  // entity doesn't exists at all
        uint e = em.createEntity();         // create one
        CHECK_NOTHROW(em.deleteEntity(e));  // delete it
        CHECK_THROWS(em.deleteEntity(e));   // it exists but is already labeled as "deleted"
    }
    
    SUBCASE("Accessing entities")
    {
        CHECK_THROWS(em[0]);            // cannot access entity #0
        CHECK_THROWS(em[123]);          // accessing non existing entity
        
        uint e = em.createEntity();
        CHECK_NOTHROW(em[e]);           // accessing existing entity
        CHECK(em[e].id == e);           // check if it return the correct entity
        
        em.deleteEntity(e);
        CHECK_THROWS(em[e]);            // accessing deleted entity
    }
    
    SUBCASE("Recycling entitites")
    {
        uint e1 = em.createEntity();    // create entities
        uint e2 = em.createEntity();
        
        CHECK(em.exists(e1) == true);   // exists is true if the entity is allocated, and not flagged "deleted" for recycling
        CHECK(em.exists(e2) == true);  
        
        em.deleteEntity(e1);            // entity will be flagged as "deleted" -> marked for recycling 
        CHECK(em.exists(e1) == false);
        
        uint e3 = em.createEntity();    // will use a recycled entity if possible, here e1
        CHECK(e1 == e3);
    }
    
    SUBCASE("Entity Signatures")
    {
        uint e1 = em.createEntity();
        em.setComponentSignature(3, e1);
        
        CHECK(em.has(3, e1));
        CHECK_FALSE(em.has(4, e1));
    }
    
    SUBCASE("Filtering Entities")
    {
        uint e1 = em.createEntity();
        uint e2 = em.createEntity();
        
        em.setComponentSignature(3, e1);        // register component of index 3
        em.setComponentSignature(3, e2);        // register component of index 3
        em.setComponentSignature(4, e2);
        
        Signature s1 = em[e1].signature;        // has 3
        Signature s2 = em[e2].signature;        // has 3 and 4
        
        CHECK(em.filter(s1).size() == 2);
        CHECK(em.filter(s2).size() == 1);
        
    }
}