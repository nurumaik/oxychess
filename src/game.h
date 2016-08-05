#pragma once

#include "oxygine-framework.h"

extern oxygine::Resources gameResources; // Idk how to make shared game resources proper way

void game_preinit();
void game_init();
void game_destroy();
void game_update();
