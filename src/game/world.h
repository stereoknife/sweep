#pragma once

#include "gameobject.h"
#include <vector>
#include "util/types.h"
#include <type_traits>

class World : Gameobject {
public:
    World();
    ~World();

    virtual void update();
    virtual void draw();
    u32 register_object(Gameobject* go);
    void delete_object(int index);
    void annihilate();

    // You hate to see it
    template<typename T>
    T* create_object() {
        static_assert(std::is_base_of_v<Gameobject, T>, "Can only register gameobjects");
        T* instance = new T();
        register_object(instance);
        return instance;
    };

private:
    std::vector<Gameobject*> game_objects;
    std::vector<int> freed_indices;
};
