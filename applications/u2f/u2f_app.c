#include "u2f_app_i.h"
#include <furi.h>
#include <furi_hal.h>

static bool u2f_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    U2fApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool u2f_app_back_event_callback(void* context) {
    furi_assert(context);
    U2fApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void u2f_app_tick_event_callback(void* context) {
    furi_assert(context);
    U2fApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

U2fApp* u2f_app_alloc() {
    U2fApp* app = furi_alloc(sizeof(U2fApp));

    app->gui = furi_record_open("gui");
    app->notifications = furi_record_open("notification");

    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&u2f_scene_handlers, app);
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, u2f_app_tick_event_callback, 500);

    view_dispatcher_set_custom_event_callback(app->view_dispatcher, u2f_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, u2f_app_back_event_callback);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->u2f_view = u2f_view_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, U2fAppViewMain, u2f_view_get_view(app->u2f_view));

    scene_manager_next_scene(app->scene_manager, U2fAppViewMain);

    return app;
}

void u2f_app_free(U2fApp* app) {
    furi_assert(app);

    // Views
    view_dispatcher_remove_view(app->view_dispatcher, U2fAppViewMain);
    u2f_view_free(app->u2f_view);

    // View dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    // Close records
    furi_record_close("gui");
    furi_record_close("notification");

    free(app);
}

int32_t u2f_app(void* p) {
    U2fApp* u2f_app = u2f_app_alloc();

    view_dispatcher_run(u2f_app->view_dispatcher);

    u2f_app_free(u2f_app);

    return 0;
}
