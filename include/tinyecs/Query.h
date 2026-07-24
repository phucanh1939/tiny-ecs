#pragma once

namespace tinyecs
{
    // Represents a view over entities that contain the requested component types.
    //
    // Query does not create or own a separate list of matching entities. Instead,
    // it provides an interface for iterating directly over the matching entities
    // in the World's storage.
    //
    // This avoids creating a temporary result list and copying matching entities
    // every time a query is executed. This is especially important for queries
    // that run frequently, such as once per frame.
    //
    // Conceptually:
    //
    //     List-based query:
    //         Find entities → Create result list → Copy entities → Iterate list
    //
    //     Iterator-based query:
    //         Find entities → Iterate matching entities directly
    //
    // For example:
    //
    //     Query<Position, Velocity> query;
    //
    //     for (Entity entity : query)
    //     {
    //         // Process entities that have both Position and Velocity.
    //     }
    //
    // A range-based for loop requires begin() and end(). The compiler uses them
    // to obtain an Iterator, then repeatedly:
    //
    //     1. Dereferences the iterator to get the current entity.
    //     2. Advances the iterator to the next matching entity.
    //     3. Compares the iterator with end() to determine whether iteration
    //        is complete.
    //
    // The Iterator represents the current position while traversing the matching
    // entities. The actual implementation will determine how the next matching
    // entity is found.
    template<typename... TComponents>
    class Query
    {
    public:
        class Iterator;

        Iterator begin();
        Iterator end();
    };
}