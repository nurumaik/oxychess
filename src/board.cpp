#include "oxygine-framework.h"
#include <functional>
using namespace oxygine;

Resources gameResources;

class MainActor: public Actor
{
public:
    std::string init_board[8] = {
        "rnbqkbnr",
        "pppppppp",
        "........",
        "........",
        "........",
        "........",
        "PPPPPPPP",
        "RNBQKBNR"
    };

    spSprite createFigure(char c) {
        // A bit inefficient, but very clear
        std::unordered_map<char, int> charToFigure = {
            {'p', 0},
            {'r', 1},
            {'n', 2},
            {'b', 3},
            {'q', 4},
            {'k', 5} 
        };
        int col = charToFigure[tolower(c)];
        int row = isupper(c) ? 1 : 0;
        spSprite figure = new Sprite();
        figure->setResAnim(gameResources.getResAnim("chess_pieces"), col, row);
        return figure;
    }

    const float boardx = 100;
    const float boardy = 100;

    spSprite figures[8][8];

    AffineTransform cellToBoard = AffineTransform::getIdentity()
        .translated({ 2, 2 }) // Board margins
        .scaled({ 32, 24 });  // Board cell sizes
    AffineTransform boardToCell = cellToBoard.inverted(); 
    AffineTransform cellToFigure = cellToBoard * AffineTransform::getIdentity()
        .translated({ 0, -12 }); // Some shifting for figures

    enum GameState {
        IDLE,
        FIGURE_SELECTED,
        FIGURE_MOVING
    };

    GameState state;
    int selx;
    int sely;
    spSprite board;

    MainActor()
    {
        board = new Sprite();
        state = IDLE;
        board->setResAnim(gameResources.getResAnim("chess_board"));
        board->setPosition(boardx, boardy);
        board->addClickListener(CLOSURE(this, &MainActor::handleBoardClick));
        board->setTouchChildrenEnabled(false);
        addChild(board);

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                char fig = init_board[j][i];
                if (fig == '.') {
                    figures[i][j] = nullptr;
                    continue;
                }
                spSprite figure = createFigure(fig);
                figure->setPosition(cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) }));
                board->addChild(figure);
                figures[i][j] = figure;
            }
        }
    }

    void handleBoardClick(Event* e) {
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
                board->removeChild(figures[i][j]);
                figures[i][j] = nullptr;
            }
            figures[selx][sely]->setPosition(cellToFigure.transform({ static_cast<float>(i), static_cast<float>(j) }));
            figures[i][j] = figures[selx][sely];
            figures[selx][sely] = nullptr;
            state = IDLE;
            break;
        }
    }
};

//declare spMainActor as intrusive_ptr holder of MainActor
typedef oxygine::intrusive_ptr<MainActor> spMainActor;
//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

void board_preinit() {}

//called from main.cpp
void board_init()
{
    //load xml file with resources definition
    gameResources.loadXML("res.xml");


    //lets create our client code simple actor
    //spMainActor was defined above as smart intrusive pointer (read more: http://www.boost.org/doc/libs/1_60_0/libs/smart_ptr/intrusive_ptr.html)
    spMainActor actor = new MainActor;

    //and add it to Stage as child
    getStage()->addChild(actor);
}


//called each frame from main.cpp
void board_update()
{
}

//called each frame from main.cpp
void board_destroy()
{
    //free previously loaded resources
    gameResources.free();
}
