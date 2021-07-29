#include "board.h"
#include <array>

board::board() : tilemap("assets/sprites/maptiles.png") {
    
}

cell* board::get(u8 x, u8 y) {
    // Check in bounds
    if (x >= SIZE || y >= SIZE) {
        return nullptr; 
    }
    return &cells[y * SIZE + x];
}

void board::clean() {
    for (u8 y = 0; y < SIZE; ++y) {
        for (u8 x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            c->mine = false;
            c->revealed = false;
            c->value = 0;
        }
    }
}

void board::randomize() {
    for (u8 i = 0; i < 15 + rand() % 10; ++i) {
        cell* c;
        u8 x, y;

        do {
          x = rand() % SIZE;
          y = rand() % SIZE;
          c = get(x, y);
        } while (c->mine == true);

        c->mine = true;

        if (x > 0) {
            get(x-1,y)->value++;
            if (y > 0)      { get(x-1,y-1)->value++; }
            if (y < SIZE-1) { get(x-1,y+1)->value++; }
        }

        if (x < SIZE-1) {
            get(x+1,y)->value++;
            if (y > 0)      { get(x+1,y-1)->value++; }
            if (y < SIZE-1) { get(x+1,y+1)->value++; }
        }

        if (y > 0)      { get(x,y-1)->value++; }
        if (y < SIZE-1) { get(x,y+1)->value++; }
    }
}

void board::draw() {; // TODO Put these somewhere logical
    constexpr u8 TILESIZE = 64; // TODO pls pls
    constexpr u8 TILES = 8; // TODO >:(

    for (u8 y = 0; y < SIZE; ++y) {
        for (u8 x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            u8 tile {0};
            if (c->revealed) {
                // tile = (x + y) % 3; // Pretty much deterministic random
            } else if (c->mine) {
                tile = 3; // + ((x ^ y) % 2);
            } else if (!c->value) {
                tile = 13;
            } else {
                tile = 4 + c->value;
            }
            
            f32 tile_x = (tile % TILES) * TILESIZE;
            f32 tile_y = (tile / TILES) * TILESIZE;
            
            tilemap.Draw(raylib::Rectangle{tile_x, tile_y, TILESIZE, TILESIZE}, {x * 32.f * 1.f, y * 32.f* 1.f, 32, 32});
        }
    }
}
