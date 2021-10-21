#pragma once

#include "drawable.h"
#include "util/types.h"

class Gameobject : public Drawable {
public:
    void draw() override = 0;
    virtual void update() = 0;
    virtual ~Gameobject() = default;

protected:
    static Gameobject* null;
};
