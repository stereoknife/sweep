#pragma once

#include <array>
#include <random>
#include <utility>
#include <type_traits>
#include <limits>
#include "util/types.h"

using cell_index = u8;

struct cell_pos {
    cell_index x;
    cell_index y;
};

struct cell {
    bool revealed {false}; // Has this tile been revealed
    bool mine {false}; // Is this tile a mine
    u8 value {0}; // 0 is no value, 1-8 is ... 1-8
    u8 draw_data {0}; // Depends on what kind of tile it is...
};

struct board {
    static constexpr cell_index SIZE {10}; // TODO Dynamic size boards and shit
    static constexpr u8 MINES {15};
    static constexpr u8 TILESIZE {64}; // TODO pls pls
    static constexpr u8 TILES {8}; // TODO >:(
    static constexpr cell_index SENTINEL {std::numeric_limits<cell_index>::max()};
        
    // x * y cells. x is horizontal, y is vertical. Cells are arranged left to right, top to bottom
    raylib::Texture tilemap;
    
    std::array<cell, SIZE * SIZE> cells {};
    
    // 0xFF means nothing selected. Effectively this cuts off value 255 from being selected
    // while we use u8 to index things
    cell_pos selected{SENTINEL, SENTINEL};
    u8 unrevealed{SIZE * SIZE};
    
    board();
    
    cell* get(cell_index x, cell_index y);
    
    vec2     tile_to_screen(cell_index x, cell_index y);
    vec2     tile_to_screen(cell_pos pos);
    cell_pos screen_to_tile(f32 x, f32 y);
    cell_pos screen_to_tile(vec2 pos);
    
    void clean();
    void randomize();
    void reveal(cell_index x, cell_index y, bool user_reveal);
    void finish();
    
    void update();
    void draw();
};
