#pragma once
#include "oxygine-framework.h"

class Figure: public oxygine::Sprite {
    static std::unordered_map<char, int> charToFigure;
public:
    enum FColor {
        WHITE,
        BLACK
    };

    FColor fcolor();
    Figure(char c); 
private:
    FColor mColor;
};

DECLARE_SMART(Figure, spFigure)