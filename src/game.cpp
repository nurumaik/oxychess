#include "oxygine-framework.h"
#include <functional>
#include "game.h"
#include "board.h"

using namespace oxygine;

Resources gameResources;

class MainActor: public Actor
{
public:

    const float boardx = 100;
    const float boardy = 100;

    spSprite board;

    MainActor()
    {
        board = new Board();
        board->setPosition(boardx, boardy);
        addChild(board);
    }

};

//declare spMainActor as intrusive_ptr holder of MainActor
typedef oxygine::intrusive_ptr<MainActor> spMainActor;
//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

void game_preinit() {}

//called from main.cpp
void game_init()
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
void game_update()
{
}

//called each frame from main.cpp
void game_destroy()
{
    //free previously loaded resources
    gameResources.free();
}
