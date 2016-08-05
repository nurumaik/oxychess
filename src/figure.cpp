#include "figure.h"
#include "game.h"
using namespace oxygine;

std::unordered_map<char, int> Figure::charToFigure = {
        {'p', 0},
        {'r', 1},
        {'n', 2},
        {'b', 3},
        {'q', 4},
        {'k', 5} 
};

Figure::Figure(char c) {
    // A bit inefficient initialization, but very clear
    int col = charToFigure[tolower(c)];
    int row = isupper(c) ? 1 : 0;
    mColor = isupper(c) ? WHITE : BLACK;

    setResAnim(gameResources.getResAnim("chess_pieces"), col, row);
}

Figure::FColor Figure::fcolor() {
    return mColor;
}
