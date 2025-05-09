#include <furi.h>
#include <gui/gui.h>
#include <gui/icon_i.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/menu.h>
#include <gui/modules/popup.h>

/* generated by fbt from .png files in images folder */
#include <qty_test_icons.h>

#define TAG "qty-test-app"

/* generated by fbt from .png files in images folder */
#include <qty_test_icons.h>

/** ids for all scenes used by the app */
typedef enum {
    TestAppScene_MainMenu,
    TestAppScene_FirstPopup,
    TestAppScene_SecondPopup,
    TestAppScene_count
} TestAppScene;

/** ids for the 2 types of view used by the app */
typedef enum { TestAppView_Menu, TestAppView_Popup } TestAppView;

/** the app context struct */
typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Menu* menu;
    Popup* popup;
} TestApp;

/** all custom events */
typedef enum { TestAppEvent_ShowPopupOne, TestAppEvent_ShowPopupTwo } TestAppEvent;

/* main menu scene */

/** indices for menu items */
typedef enum { TestAppMenuSelection_One, TestAppMenuSelection_Two } TestAppMenuSelection;

/** main menu callback - sends a custom event to the scene manager based on the menu selection */
void qty_test_menu_callback_main_menu(void* context, uint32_t index) {
    FURI_LOG_T(TAG, "qty_test_menu_callback_main_menu");
    TestApp* app = context;
    switch(index) {
    case TestAppMenuSelection_One:
        scene_manager_handle_custom_event(app->scene_manager, TestAppEvent_ShowPopupOne);
        break;
    case TestAppMenuSelection_Two:
        scene_manager_handle_custom_event(app->scene_manager, TestAppEvent_ShowPopupTwo);
        break;
    }
}

/** resets the menu, gives it content, callbacks and selection enums */
void qty_test_scene_on_enter_main_menu(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_enter_main_menu");
    TestApp* app = context;
    menu_reset(app->menu);

    // NB. icons are specified as NULL below, because:
    // * icons are _always_ animated by the menu
    // * the icons provided (&I_one, &I_two) are generated by the build process
    // * these icons do not have a framerate (resulting in a division by zero)
    menu_add_item(
        app->menu,
        "First popup",
        NULL,
        TestAppMenuSelection_One,
        qty_test_menu_callback_main_menu,
        app);
    menu_add_item(
        app->menu,
        "Second popup",
        NULL,
        TestAppMenuSelection_Two,
        qty_test_menu_callback_main_menu,
        app);
    view_dispatcher_switch_to_view(app->view_dispatcher, TestAppView_Menu);
}

/** main menu event handler - switches scene based on the event */
bool qty_test_scene_on_event_main_menu(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "qty_test_scene_on_event_main_menu");
    TestApp* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case TestAppEvent_ShowPopupOne:
            scene_manager_next_scene(app->scene_manager, TestAppScene_FirstPopup);
            consumed = true;
            break;
        case TestAppEvent_ShowPopupTwo:
            scene_manager_next_scene(app->scene_manager, TestAppScene_SecondPopup);
            consumed = true;
            break;
        }
        break;
    default: // eg. SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void qty_test_scene_on_exit_main_menu(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_exit_main_menu");
    TestApp* app = context;
    menu_reset(app->menu);
}

/* popup 1 scene */

void qty_test_scene_on_enter_popup_one(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_enter_popup_one");
    TestApp* app = context;
    popup_reset(app->popup);
    popup_set_context(app->popup, app);
    popup_set_header(app->popup, "Popup One", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "One! One popup. Ah ah ah...", 64, 20, AlignLeft, AlignTop);
    view_dispatcher_switch_to_view(app->view_dispatcher, TestAppView_Popup);
}

bool qty_test_scene_on_event_popup_one(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "qty_test_scene_on_event_popup_one");
    UNUSED(context);
    UNUSED(event);
    return false; // don't handle any events
}

void qty_test_scene_on_exit_popup_one(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_exit_popup_one");
    TestApp* app = context;
    popup_reset(app->popup);
}

/* popup 2 scene */

void qty_test_scene_on_enter_popup_two(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_enter_popup_two");
    TestApp* app = context;
    popup_reset(app->popup);
    popup_set_context(app->popup, app);
    popup_set_header(app->popup, "Popup Two", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Two! Two popups. (press back)", 64, 20, AlignLeft, AlignTop);
    view_dispatcher_switch_to_view(app->view_dispatcher, TestAppView_Popup);
}

bool qty_test_scene_on_event_popup_two(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "qty_test_scene_on_event_popup_two");
    UNUSED(context);
    UNUSED(event);
    return false; // don't handle any events
}

void qty_test_scene_on_exit_popup_two(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_on_exit_popup_two");
    TestApp* app = context;
    popup_reset(app->popup);
}

/** collection of all scene on_enter handlers - in the same order as their enum */
void (*const qty_test_scene_on_enter_handlers[])(void*) = {
    qty_test_scene_on_enter_main_menu,
    qty_test_scene_on_enter_popup_one,
    qty_test_scene_on_enter_popup_two};

/** collection of all scene on event handlers - in the same order as their enum */
bool (*const qty_test_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    qty_test_scene_on_event_main_menu,
    qty_test_scene_on_event_popup_one,
    qty_test_scene_on_event_popup_two};

/** collection of all scene on exit handlers - in the same order as their enum */
void (*const qty_test_scene_on_exit_handlers[])(void*) = {
    qty_test_scene_on_exit_main_menu,
    qty_test_scene_on_exit_popup_one,
    qty_test_scene_on_exit_popup_two};

/** collection of all on_enter, on_event, on_exit handlers */
const SceneManagerHandlers qty_test_scene_event_handlers = {
    .on_enter_handlers = qty_test_scene_on_enter_handlers,
    .on_event_handlers = qty_test_scene_on_event_handlers,
    .on_exit_handlers = qty_test_scene_on_exit_handlers,
    .scene_num = TestAppScene_count};

/** custom event handler - passes the event to the scene manager */
bool qty_test_scene_manager_custom_event_callback(void* context, uint32_t custom_event) {
    FURI_LOG_T(TAG, "qty_test_scene_manager_custom_event_callback");
    furi_assert(context);
    TestApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

/** navigation event handler - passes the event to the scene manager */
bool qty_test_scene_manager_navigation_event_callback(void* context) {
    FURI_LOG_T(TAG, "qty_test_scene_manager_navigation_event_callback");
    furi_assert(context);
    TestApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

/** initialise the scene manager with all handlers */
void qty_test_scene_manager_init(TestApp* app) {
    FURI_LOG_T(TAG, "qty_test_scene_manager_init");
    app->scene_manager = scene_manager_alloc(&qty_test_scene_event_handlers, app);
}

/** initialise the views, and initialise the view dispatcher with all views */
void qty_test_view_dispatcher_init(TestApp* app) {
    FURI_LOG_T(TAG, "qty_test_view_dispatcher_init");
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    // allocate each view
    FURI_LOG_D(TAG, "qty_test_view_dispatcher_init allocating views");
    app->menu = menu_alloc();
    app->popup = popup_alloc();

    // assign callback that pass events from views to the scene manager
    FURI_LOG_D(TAG, "qty_test_view_dispatcher_init setting callbacks");
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, qty_test_scene_manager_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, qty_test_scene_manager_navigation_event_callback);

    // add views to the dispatcher, indexed by their enum value
    FURI_LOG_D(TAG, "qty_test_view_dispatcher_init adding view menu");
    view_dispatcher_add_view(app->view_dispatcher, TestAppView_Menu, menu_get_view(app->menu));

    FURI_LOG_D(TAG, "qty_test_view_dispatcher_init adding view popup");
    view_dispatcher_add_view(app->view_dispatcher, TestAppView_Popup, popup_get_view(app->popup));
}

/** initialise app data, scene manager, and view dispatcher */
TestApp* qty_test_init() {
    FURI_LOG_T(TAG, "qty_test_init");
    TestApp* app = malloc(sizeof(TestApp));
    qty_test_scene_manager_init(app);
    qty_test_view_dispatcher_init(app);
    return app;
}

/** free all app data, scene manager, and view dispatcher */
void qty_test_free(TestApp* app) {
    FURI_LOG_T(TAG, "qty_test_free");
    scene_manager_free(app->scene_manager);
    view_dispatcher_remove_view(app->view_dispatcher, TestAppView_Menu);
    view_dispatcher_remove_view(app->view_dispatcher, TestAppView_Popup);
    view_dispatcher_free(app->view_dispatcher);
    menu_free(app->menu);
    popup_free(app->popup);
    free(app);
}

/** go to trace log level in the dev environment */
void qty_test_set_log_level() {
#ifdef FURI_DEBUG
    furi_log_set_level(FuriLogLevelTrace);
#else
    furi_log_set_level(FuriLogLevelInfo);
#endif
}

int32_t qty_test_app(void* p) {
	UNUSED(p);
    qty_test_set_log_level();

    // create the app context struct, scene manager, and view dispatcher
    FURI_LOG_I(TAG, "Test app starting...");
    TestApp* app = qty_test_init();

    // set the scene and launch the main loop
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, TestAppScene_MainMenu);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    // free all memory
    FURI_LOG_I(TAG, "Test app finishing...");
    furi_record_close(RECORD_GUI);
    qty_test_free(app);
    return 0;
}
