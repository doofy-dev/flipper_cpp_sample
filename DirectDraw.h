#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/canvas_i.h>
#include <input/input.h>

struct DirectDraw{
    FuriPubSub* input;
    FuriPubSubSubscription* input_subscription;
    Gui* gui;
    Canvas* canvas;
    bool stop;
    uint32_t counter;
};