#include "board.h"
#include <array>
#include <iostream>

Board::Board() : tilemap("assets/sprites/maptiles.png") {
    reset();
}


void Board::update() {
    if (IsKeyPressed(KEY_K)) {
        reset();
    }
    
    auto [sel_x, sel_y] = screen_to_tile(GetMousePosition());
    cell* c = get(sel_x, sel_y);
    if (c) {
        selected = {sel_x, sel_y};
        
        if (done) return;
        
        if (raylib::Mouse::IsButtonReleased(MOUSE_RIGHT_BUTTON)) {
            if (!c->revealed) {
                c->draw_data = c->draw_data ? 0 : 2; // Toggle 0 to 2 to 0
            } else if (!c->mine) {
                u8 value = c->value;
                u8 marked = 0;
                do_surrounding(sel_x, sel_y, [&marked](auto, auto, cell* c){ 
                    marked += (!c->revealed && c->draw_data > 0);
                });

                if (marked >= value) {
                    do_surrounding(sel_x, sel_y, [this](auto x, auto y, auto) {
                        reveal(x, y, true);
                    });
                }
            }
        }
        
        if (raylib::Mouse::IsButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (!generated) {
                generate(sel_x, sel_y);
            }
            reveal(sel_x, sel_y, true);
        }
        
    } else {
        selected = {0xFF, 0xFF};
    }
}

void Board::draw() {    
    for (cell_index y = 0; y < SIZE; ++y) {
        for (cell_index x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            u8 tile {0};
            if (!c->revealed) {
                tile = c->draw_data;
            } else if (c->mine) {
                tile = 3 + c->draw_data;
            } else if (!c->value) {
                tile = 13;
            } else {
                tile = 4 + c->value;
            }
            
            auto [draw_x, draw_y] = tile_to_screen(x, y);
            f32 tile_x = (tile % TILES) * TILESIZE;
            f32 tile_y = (tile / TILES) * TILESIZE;
            
            // TODO This 32 depends on zoom? Can we zoom?
            tilemap.Draw(rect{tile_x, tile_y, TILESIZE, TILESIZE}, {draw_x, draw_y, TILESIZE, TILESIZE}); 
        }
    }
    
    if (selected.x != SENTINEL) {
        auto [draw_x, draw_y] = tile_to_screen(selected);
        DrawRectangle(draw_x+1, draw_y+1, TILESIZE-2, TILESIZE-2, {255, 255, 255, 128});
    }
}

cell* Board::get(cell_index x, cell_index y) {
    // Check in bounds
    if (x >= SIZE || y >= SIZE) {
        return nullptr; 
    }
    return &cells[y * SIZE + x];
}

cell_index Board::distance(cell_index x1, cell_index y1, cell_index x2, cell_index y2) {
    return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
}

// These functions will get more complicated once dynamic maps and scrolling are added, 
// but for now they're good like this
vec2 Board::tile_to_screen(cell_index x, cell_index y) {
    return {(f32) x * TILESIZE, (f32) y * TILESIZE};
}

vec2 Board::tile_to_screen(cell_pos pos) {
    return tile_to_screen(pos.x, pos.y);
}

cell_pos Board::screen_to_tile(f32 x, f32 y) {
    return {(cell_index) (x / TILESIZE), (cell_index) (y / TILESIZE)}; 
}

cell_pos Board::screen_to_tile(vec2 pos) {
    return screen_to_tile(pos.x, pos.y); 
}

void Board::reset() {
    for (cell_index y = 0; y < SIZE; ++y) {
        for (cell_index x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            c->mine = false;
            c->revealed = false;
            c->value = 0;
            c->draw_data = 0;
        }
    }
    unrevealed = SIZE * SIZE;
    generated = false;
    done = false;
}

void Board::generate(cell_index sx, cell_index sy) {
    for (u8 i = 0; i < 15 + rand() % 10; ++i) {
        cell* c;
        cell_index x, y;

        do {
            x = rand() % SIZE;
            y = rand() % SIZE;
            c = get(x, y);
        } while (c->mine || distance(x, y, sx, sy) <= 1);

        c->mine = true;

        // TODO Kami...
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
    generated = true;
}

void Board::reveal(cell_index x, cell_index y, bool user_reveal) {
    cell* c = get(x, y);
    if (!c || c->revealed || (c->draw_data > 0 && user_reveal)) {
        return;
    }
    
    c->revealed = true;
    c->draw_data = 0;
    
    if (c->mine && user_reveal) {
        c->draw_data = 1;
        lose();
    }
    
    unrevealed--;
    if (!c->mine) { // Number
        if (c->value == 0 && user_reveal) {
            reveal(x-1, y-1, true);
            reveal(x-1, y, true);
            reveal(x-1, y+1, true);
            reveal(x, y-1, true);
            reveal(x, y+1, true);
            reveal(x+1, y-1, true);
            reveal(x+1, y, true);
            reveal(x+1, y+1, true);
        }
        
        if (unrevealed == MINES) {
            win();
        }
    }
    
    return;
}

void Board::finish() {
    for (cell_index y = 0; y < SIZE; ++y) {
        for (cell_index x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            if (!c->revealed) {
                c->draw_data = 0;
            }
            c->revealed = true;
        }
    }
}

void Board::win() {
    finish();
    std::cout << "You win :)";
    done = true;
}

void Board::lose() {
    finish();
    std::cout << "Oh no :("; // TODO Logger class
    done = true;
}

void Board::do_surrounding(cell_index x, cell_index y, std::function<void(cell_index, cell_index, cell*)> f) {
    for (s8 xx = -1; xx <= 1; ++xx) {
        for (s8 yy = -1; yy <= 1; ++yy) {
            cell* c = get(x + xx, y + yy);
            if (!(x | y) || !c) continue;
            f(x + xx, y + yy, c);
        }
    }
}
