#pragma once

#include "game/gameobject.h"
#include "util/types.h"
#include "board/board.h"

class Character: public Gameobject {
public:
    u16 x;
    u16 y;
    u16 hp;

    void move(int x, int y);
    virtual void update();
    virtual void draw();

    Character ();

private:
    Board* board;
};
