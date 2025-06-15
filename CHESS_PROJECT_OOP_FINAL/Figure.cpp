#include "Figure.h"
#pragma execution_character_set("utf-8")


Figure::Figure(Color c) : color(c), hasMoved(false) {}

Figure::~Figure() {}

Color Figure::getColor() const {
    return color; 
}

