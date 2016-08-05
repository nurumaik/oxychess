#pragma once
#include "oxygine-framework.h"

class Board: public oxygine::Sprite {
    static std::string init_board[8];
    static oxygine::AffineTransform cellToBoard;
    static oxygine::AffineTransform boardToCell; 
    static oxygine::AffineTransform cellToFigure; // Some shifting for figures

    oxygine::spSprite figures[8][8];
    enum GameState {
        IDLE,
        FIGURE_SELECTED,
        FIGURE_MOVING
    };
    GameState state;
    int selx;
    int sely;

    
    void handleBoardClick(oxygine::Event* e);
public:
    Board();
};