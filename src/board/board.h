#pragma once

#include <array>
#include <random>
#include "util/types.h"

struct cell {
    bool revealed {false}; // Has this tile been revealed
    bool mine {false}; // Is this tile a mine
    u8 value {0}; // 0 is no value, 1-8 is ... 1-8
};

struct board {
    static constexpr u8 SIZE {10}; // TODO Dynamic size boards and shit
    static constexpr u8 MINES {15};
    // x * y cells. x is horizontal, y is vertical. Cells are arranged left to right, top to bottom
    raylib::Texture tilemap;
    
    std::array<cell, SIZE * SIZE> cells {};
    
    board();
    
    cell* get(u8 x, u8 y);
    
    void clean();

    void randomize();
    
    void draw();
};
