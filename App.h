#pragma once
#include "furi.h"
#include "DirectDraw.h"

class App {
    DirectDraw* instance;
    static void input_callback(const void* value, void *ctx);
    void draw_block(uint32_t size, uint32_t counter);
public:
    void run();
    App();
    ~App();
};
