#include "App.h"
#include <furi.h>
#include <gui/gui.h>
#include <gui/canvas_i.h>
#include <input/input.h>

#define BUFFER_SIZE (32U)

void App::run() {
    size_t start = DWT->CYCCNT;
    size_t counter = 0;
    float fps = 0;

    vTaskPrioritySet(static_cast<TaskHandle_t>(furi_thread_get_current_id()), FuriThreadPriorityIdle);

    do {
        size_t elapsed = DWT->CYCCNT - start;
        char buffer[BUFFER_SIZE] = {0};

        if (elapsed >= 64000000) {
            fps = (float) counter / ((float) elapsed / 64000000.0f);

            start = DWT->CYCCNT;
            counter = 0;
        }
        snprintf(buffer, BUFFER_SIZE, "FPS: %.1f", (double) fps);

        canvas_reset(instance->canvas);
        canvas_set_color(instance->canvas, ColorXOR);
        draw_block(instance->counter % 16, instance->counter);
        draw_block(instance->counter * 2 % 16, instance->counter * 2);
        draw_block(instance->counter * 3 % 16, instance->counter * 3);
        draw_block(instance->counter * 4 % 16, instance->counter * 4);
        draw_block(instance->counter * 5 % 16, instance->counter * 5);
        canvas_draw_str(instance->canvas, 10, 10, buffer);
        canvas_commit(instance->canvas);

        counter++;
        instance->counter++;
        furi_thread_yield();
    } while (!instance->stop);
}

void App::input_callback(const void *value, void *ctx) {
    furi_assert(value);
    furi_assert(ctx);

    DirectDraw* instance = static_cast<DirectDraw *>(ctx);
    const InputEvent* event = static_cast<const InputEvent *>(value);

    if(event->key == InputKeyBack && event->type == InputTypeShort) {
        instance->stop = true;
    }
}

App::~App() {
    furi_pubsub_unsubscribe(instance->input, instance->input_subscription);

    instance->canvas = NULL;
    gui_direct_draw_release(instance->gui);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_INPUT_EVENTS);
}

App::App() {
    instance = new DirectDraw;

    instance->input = static_cast<FuriPubSub *>(furi_record_open(RECORD_INPUT_EVENTS));
    instance->gui = static_cast<Gui *>(furi_record_open(RECORD_GUI));
    instance->canvas = gui_direct_draw_acquire(instance->gui);

    instance->input_subscription = furi_pubsub_subscribe(instance->input, &input_callback, instance);
}

void App::draw_block(uint32_t size, uint32_t counter) {
    size += 16;
    uint8_t width = canvas_width(instance->canvas) - size;
    uint8_t height = canvas_height(instance->canvas) - size;

    uint8_t x = counter % width;
    if ((counter / width) % 2) {
        x = width - x;
    }

    uint8_t y = counter % height;
    if ((counter / height) % 2) {
        y = height - y;
    }

    canvas_draw_box(instance->canvas, x, y, size, size);
}
