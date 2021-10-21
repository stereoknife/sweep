#include "world.h"
#include "util/types.h"

World::World() {}
World::~World() {
    annihilate();
}

void World::update() {
    for (u32 i = 0; i < game_objects.size(); ++i) {
        if (game_objects[i]) game_objects[i]->update();
    }
}

void World::draw() {
    for (u32 i = 0; i < game_objects.size(); ++i) {
        if (game_objects[i]) game_objects[i]->draw();
    }
}

u32 World::register_object(Gameobject* go) {
    if (freed_indices.size() > 0) {
        u32 index = freed_indices.back();
        freed_indices.pop_back();
        game_objects[index] = go;
        return index;
    } else {
        game_objects.push_back(go);
        return game_objects.size() - 1;
    }
}

void World::delete_object(u32 index) {

}

void World::annihilate() {
    for (int i = 0; i < game_objects.size(); ++i) {
        delete game_objects[i];
    }
}
