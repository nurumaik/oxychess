#pragma once
#include "figure.h"
#include "oxygine-framework.h"

class Board: public oxygine::Sprite {
    static std::string init_board[8];
    static oxygine::AffineTransform cellToBoard;
    static oxygine::AffineTransform boardToCell; 
    static oxygine::AffineTransform cellToFigure;

    enum GameState {
        IDLE,
        FIGURE_SELECTED,
        FIGURE_MOVING
    };

    GameState mState;
    Figure::FColor mTurn;
    spFigure mFigures[8][8];
    int mSeli; // Coordinates of currently selected figure
    int mSelj;
    
    void handleBoardClick(oxygine::Event* e);
public:
    Board();
};

DECLARE_SMART(Board, spBoard)