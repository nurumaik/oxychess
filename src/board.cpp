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

    MainActor()
    {
        spSprite board = new Sprite();
        board->setResAnim(gameResources.getResAnim("chess_board"));
        board->setPosition(boardx, boardy);
        addChild(board);

        auto figuresTransform = AffineTransform::getIdentity()
            .translated({ boardx + 2, boardy + 2 - 12 }) // Board margins and some y shift
            .scaled({ 32, 24 });                         // Board piece sizes

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                char fig = init_board[j][i];
                if (fig == '.')
                    continue;
                spSprite figure = createFigure(fig);
                figure->setPosition(figuresTransform.transform({ static_cast<float>(i), static_cast<float>(j) }));
                addChild(figure);
            }
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
