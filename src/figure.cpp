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
        int col = charToFigure[tolower(c)];
        int row = isupper(c) ? 1 : 0;
        setResAnim(gameResources.getResAnim("chess_pieces"), col, row);
}
