#pragma once

#include <array>
#include <random>
#include <utility>
#include <type_traits>
#include <limits>
#include <functional>
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
    
    bool is_marked();
};

class Board {
public:
    static constexpr cell_index SIZE {10}; // TODO Dynamic size boards and shit
    static constexpr u8 MINES {15};
    static constexpr u8 TILESIZE {64}; // TODO pls pls
    static constexpr u8 TILES {8}; // TODO >:(
    static constexpr cell_index SENTINEL {std::numeric_limits<cell_index>::max()};
        
    Board();
    
    void update();
    void draw();
    
    cell* get(cell_index x, cell_index y);
    cell_index distance(cell_index x1, cell_index y1, cell_index x2, cell_index y2);
    
    vec2     tile_to_screen(cell_index x, cell_index y);
    vec2     tile_to_screen(cell_pos pos);
    cell_pos screen_to_tile(f32 x, f32 y);
    cell_pos screen_to_tile(vec2 pos);

private:
    void reset();
    void generate(cell_index sx, cell_index sy);
    void reveal(cell_index x, cell_index y, bool user_reveal);
    void finish();
    
    void win();
    void lose();
    
    void do_surrounding(cell_index x, cell_index y, std::function<void(cell_index x, cell_index y, cell* c)> f);
    
    // x * y cells. x is horizontal, y is vertical. Cells are arranged left to right, top to bottom
    raylib::Texture tilemap;
    
    std::array<cell, SIZE * SIZE> cells {};
    
    // 0xFF means nothing selected. Effectively this cuts off value 255 from being selected
    // while we use u8 to index things
    cell_pos selected{SENTINEL, SENTINEL};
    u8 unrevealed{SIZE * SIZE}; // TODO Move to constructor when dynamic sizes
    bool generated{false};
    bool done{false};
};
