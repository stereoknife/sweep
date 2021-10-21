#include "raylib-cpp.hpp"

#include <cstdlib>
#include <ctime>
#include <character/character.h>

#include "board/board.h"
#include "game/world.h"

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    
    std::srand(std::time(nullptr)); // TODO Better random generation
    
    int screenWidth = 1024;
    int screenHeight = 1024;
    
    raylib::Window window(screenWidth, screenHeight, "Minesweep");
    window.SetPosition(GetMonitorPosition(0));

    Board b{};
    World world{};

    world.create_object<Character>();
    
    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        // Update
        b.update();
        world.update();
        
        // Draw 
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        b.draw();
        world.draw();
        
        EndDrawing();
    }

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}
