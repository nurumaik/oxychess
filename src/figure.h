#pragma once
#include "oxygine-framework.h"

class Figure: public oxygine::Sprite {
    static std::unordered_map<char, int> charToFigure;
public:
    Figure(char c); 
};