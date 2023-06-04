#include "App.h"

#ifdef __cplusplus
extern "C"
{
#endif

int32_t flipper_cpp(void *args) {
    UNUSED(args);
    App *app = new App();
    app->run();
    delete app;

    return 0;
}

#ifdef __cplusplus
}
#endif