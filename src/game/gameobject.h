#pragma once

#include "drawable.h"
#include "util/types.h"

class Gameobject : public Drawable {
public:
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual ~Gameobject() {};

protected:
    static Gameobject* null;
};
