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
    mState = IDLE;
    setResAnim(gameResources.getResAnim("chess_board"));
    addClickListener(CLOSURE(this, &Board::handleBoardClick));
    setTouchChildrenEnabled(false);
    mSeli = 0;
    mSelj = 0;

    // Placing figure on board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char fig = init_board[j][i];
            if (fig == '.') {
                mFigures[i][j] = nullptr;
                continue;
            }
            spFigure figure = new Figure(fig);
            figure->setPosition(cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) }));
            figure->setPriority(j + 1);
            addChild(figure);
            mFigures[i][j] = figure;
        }
    }
}

void Board::handleBoardClick(Event* e) {
    // Getting board coordinates
    auto pos = boardToCell.transform(dynamic_cast<TouchEvent*>(e)->position);

    // Board coordinates of click
    int i = floor(pos.x);
    int j = floor(pos.y);

    if (i < 0 || i > 7 || j < 0 || j > 7) //Clicked on board margins
        return;

    // Expected figure screen coordinates of click (center of cell)
    pos = cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) });

    spFigure clicked_fig = mFigures[i][j];
    spFigure selected_fig = mFigures[mSeli][mSelj];

    switch (mState) {
    case IDLE:
        if (!clicked_fig)
            break;
        mSeli = i;
        mSelj = j;
        mState = FIGURE_MOVING;
        // Lifting figure a bit
        clicked_fig->addTween(Actor::TweenY(pos.y - 6), 100)
                   ->addDoneCallback([this](Event*) {mState = FIGURE_SELECTED;});
        break;

    case FIGURE_SELECTED:
        mState = FIGURE_MOVING;
        if (i == mSeli && j == mSelj) {
            // Putting figure back, cancelling turn
            clicked_fig->addTween(Actor::TweenY(pos.y), 100)
                       ->addDoneCallback([this](Event*) {mState = IDLE;});
            break;
        }

        if (clicked_fig && clicked_fig->fcolor() == selected_fig->fcolor()) {
            mState = FIGURE_SELECTED;
            break; // Friendly fire off
        }

        mFigures[i][j] = selected_fig;
        mFigures[mSeli][mSelj] = nullptr;
        selected_fig->setPriority(10);
        selected_fig->addTween(TweenQueue::create(
            createTween(Actor::TweenPosition(pos - Vector2{0, 6}), 100),
            createTween(Actor::TweenY(pos.y), 100)))
            ->addDoneCallback([this, clicked_fig, selected_fig, j](Event*) {
                // Remove destination figure
                if (clicked_fig) {
                    removeChild(clicked_fig);
                }
                selected_fig->setPriority(j + 1);
                mState = IDLE; 
            });
        break;

    case FIGURE_MOVING:
        break; // User should wait for move to finish
    }
}
