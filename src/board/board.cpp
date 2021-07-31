#include "board.h"
#include <array>

board::board() : tilemap("assets/sprites/maptiles.png") {

}

cell* board::get(cell_index x, cell_index y) {
    // Check in bounds
    if (x >= SIZE || y >= SIZE) {
        return nullptr; 
    }
    return &cells[y * SIZE + x];
}

// These functions will get more complicated once dynamic maps and scrolling are added, 
// but for now they're good like this
vec2 board::tile_to_screen(cell_index x, cell_index y) {
    return {(f32) x * TILESIZE, (f32) y * TILESIZE};
}

vec2 board::tile_to_screen(cell_pos pos) {
    return tile_to_screen(pos.x, pos.y);
}

cell_pos board::screen_to_tile(f32 x, f32 y) {
    return {(cell_index) (x / TILESIZE), (cell_index) (y / TILESIZE)}; 
}

cell_pos board::screen_to_tile(vec2 pos) {
    return screen_to_tile(pos.x, pos.y); 
}

void board::clean() {
    for (cell_index y = 0; y < SIZE; ++y) {
        for (cell_index x = 0; x < SIZE; ++x) {
            cell* c = get(x, y);
            c->mine = false;
            c->revealed = false;
            c->value = 0;
            c->draw_data = 0;
        }
    }
}

void board::randomize() {
    for (u8 i = 0; i < 15 + rand() % 10; ++i) {
        cell* c;
        cell_index x, y;

        do {
          x = rand() % SIZE;
          y = rand() % SIZE;
          c = get(x, y);
        } while (c->mine);

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

void board::reveal(cell_index x, cell_index y, bool user_reveal) {
    cell* c = get(x, y);
    if (!c || c->revealed || (c->draw_data > 0 && user_reveal)) {
        return;
    }
    
    c->revealed = true;
    c->draw_data = 0;
    
    if (c->mine && user_reveal) {
        c->draw_data = 1;
        finish();
        // Game lost uwu
    }
    
    unrevealed--;
    if (!c->mine) { // Number
        if (c->value == 0 && user_reveal) {
            reveal(x-1, y, true);
            reveal(x-1, y+1, true);
            reveal(x-1, y-1, true);
            reveal(x, y-1, true);
            reveal(x, y+1, true);
            reveal(x+1, y, true);
            reveal(x+1, y-1, true);
            reveal(x+1, y+1, true);
        }
        
        if (unrevealed == MINES) {
            finish();
        }
    }
    
    return;
}

void board::finish() {
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

void board::update() {
    if (IsKeyPressed(KEY_K)) {
        clean();
        randomize();
    }
    
    auto [sel_x, sel_y] = screen_to_tile(GetMousePosition());
    cell* c = get(sel_x, sel_y);
    if (c) {
        selected = {sel_x, sel_y};
        
        if (raylib::Mouse::IsButtonReleased(MOUSE_RIGHT_BUTTON)) {
            if (!c->revealed) {
                c->draw_data = c->draw_data ? 0 : 2; // Toggle 0 to 2 to 0
            } else if (!c->mine) {
                u8 value = c->value;
                u8 marked = 0;
                for (s8 x = -1; x <= 1; ++x) {
                    for (s8 y = -1; y <= 1; ++y) {
                        cell* c = get(sel_x + x, sel_y + y);
                        if (!(x | y) || !c) continue;
                        marked += (!c->revealed && c->draw_data > 0);
                    }
                }
                if (marked >= value) {
                    for (s8 x = -1; x <= 1; ++x) {
                        for (s8 y = -1; y <= 1; ++y) {
                            cell* c = get(sel_x + x, sel_y + y);
                            if (!(x | y) || !c) continue;
                            reveal(sel_x + x, sel_y + y, true);
                        }
                    }
                }
            }
        }
        
        if (raylib::Mouse::IsButtonReleased(MOUSE_LEFT_BUTTON)) {
            reveal(sel_x, sel_y, true);
        }
        
    } else {
        selected = {0xFF, 0xFF};
    }
}

void board::draw() {    
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
