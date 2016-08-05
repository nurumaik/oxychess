#include "board.h"
#include "figure.h"
#include "game.h"
using namespace oxygine;

// Some useful transformations to go from discrete board coords to pixels and vice versa
AffineTransform Board::cellToBoard = AffineTransform::getIdentity()
    .translated({ 2, 2 }) // Board margins
    .scaled({ 32, 24 });  // Board cell sizes
AffineTransform Board::boardToCell = Board::cellToBoard.inverted(); 
AffineTransform Board::cellToFigure = Board::cellToBoard * AffineTransform::getIdentity()
    .translated({ 0, -12 }); // Some shifting for figures

//Capital letters is white
std::string Board::init_board[8]  = {
    "rnbqkbnr",
    "pppppppp",
    "........",
    "........",
    "........",
    "........",
    "PPPPPPPP",
    "RNBQKBNR"
};

Board::Board() {
    state = IDLE;
    setResAnim(gameResources.getResAnim("chess_board"));
    addClickListener(CLOSURE(this, &Board::handleBoardClick));
    setTouchChildrenEnabled(false);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char fig = init_board[j][i];
            if (fig == '.') {
                figures[i][j] = nullptr;
                continue;
            }
            spSprite figure = new Figure(fig);
            figure->setPosition(cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) }));
            figure->setPriority(j + 1);
            addChild(figure);
            figures[i][j] = figure;
        }
    }
}

void Board::handleBoardClick(Event* e) {
    //Getting board coordinates
    auto pos = boardToCell.transform(dynamic_cast<TouchEvent*>(e)->position);
    int i = floor(pos.x);
    int j = floor(pos.y);
    //log::messageln("clicked board at %f %f == %d %d", dynamic_cast<TouchEvent*>(e)->position.x, dynamic_cast<TouchEvent*>(e)->position.y, i, j);
    if (i < 0 || i > 7 || j < 0 || j > 7) {
        return; // Nothing to do here
    }
    switch (state) {
    case FIGURE_MOVING:
        break; // User should wait for move to finish
    case IDLE:
        if (!figures[i][j])
            break;
        selx = i;
        sely = j;
        state = FIGURE_SELECTED;
        break;
    case FIGURE_SELECTED:
        if (i == selx && j == sely) {
            state = IDLE;
        }
        if (figures[i][j]) {
            removeChild(figures[i][j]);
            figures[i][j] = nullptr;
        }
        figures[selx][sely]->setPosition(cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) }));
        figures[i][j] = figures[selx][sely];
        figures[selx][sely] = nullptr;
        figures[i][j]->setPriority(j + 1);
        state = IDLE;
        break;
    }
}
