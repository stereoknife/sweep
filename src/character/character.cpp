#include "character.h"
#include <iostream>

Character::Character() {}


void Character::update() {
    if (IsKeyPressed(KEY_W)) {
        if (1 < y) y -= 1;
    }

    if (IsKeyPressed(KEY_A)) {
        if (1 < x) x -= 1;
    }

    if (IsKeyPressed(KEY_S)) {
        if (y < board->SIZE) y += 1;
    }

    if (IsKeyPressed(KEY_D)) {
        if (x < board->SIZE) x += 1;
    }
};

void Character::draw()
{
    auto [draw_x, draw_y] = board->tile_to_screen(x, y);

    // do the draw here
}

