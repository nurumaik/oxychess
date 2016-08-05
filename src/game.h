#pragma once

#include "oxygine-framework.h"

extern oxygine::Resources gameResources; // Idk how to make shared game resources proper way
const float game_scale = 1.f; //Change this to 2 or 4 if game looks too tiny

void game_preinit();
void game_init();
void game_destroy();
void game_update();
