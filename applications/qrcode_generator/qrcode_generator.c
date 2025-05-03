#include "qrencode/qrcodegen.h"
//#include "qrcode_renderer.h"
#include <furi.h>

#include <gui/gui.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_box.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Diffrent app scenes
typedef enum {
    MainMenuScene,
    QrCodeInputScene,
    QrCodeMessageScene,
    ReadmeScene,
    QrCodeSceneCount,
} QrCodeScene;

// View references
typedef enum {
    QrCodeSubmenuView,
    QrCodeWidgetView,
    QrCodeTextInputView,
    QrCodeTextBoxView,
} QrCodeView;

// App object

typedef struct App {
    SceneManager *scene_manager;
    ViewDispatcher *view_dispatcher;
    Submenu *submenu;
    Widget *widget;
    TextInput *text_input;
    TextBox *text_box;
    char *qrcode_text;
    uint8_t qrcode_text_size;
} App;

// Refence to item menus. Avoid magic numbers
typedef enum {
    MainMenuSceneQrCode,
    MainMenuSceneReadme,
} MainMenuSceneIndex;

// Reference to custom events. Avoid magic numbers
typedef enum {
    MainMenuSceneQrCodeEvent,
    MainMenuSceneReadmeEvent,
} QrCodeMainMenuEvent;

typedef enum {
    QrCodeInputSceneSaveEvent,
} QrCodeInputEvent;

// QRCode generation code
// TODO: Put correct code here
char *qrcode(const char *data) {
    if (data == NULL) {
        return NULL;
    }

    char *result = malloc(strlen(data) + 1);

    if (result != NULL) {
        strcpy(result, data);
    }

    return result;
}

// Function for stub menu
void qrcode_menu_callback(void *context, uint32_t index) {
    App *app = context;

    switch (index) {
        case MainMenuSceneReadme:
            scene_manager_handle_custom_event(app->scene_manager,
                    MainMenuSceneReadmeEvent);
            break;

        case MainMenuSceneQrCode:
            scene_manager_handle_custom_event(app->scene_manager,
                    MainMenuSceneQrCodeEvent);
            break;
    }
}

// Functions for every scene
// Every scene must have on_enter, on_event and on_exit
void qrcode_main_menu_scene_on_enter(void *context) {
    App *app = context;

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "QRCode Generator");

    submenu_add_item(app->submenu, "Generate",
            MainMenuSceneQrCode, qrcode_menu_callback, app);
    submenu_add_item(app->submenu, "README", MainMenuSceneReadme,
            qrcode_menu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, QrCodeSubmenuView);
}

bool qrcode_main_menu_scene_on_event(void *context, SceneManagerEvent event) {
    App *app = context;
    bool consumed = false;

    switch (event.type) {
        case SceneManagerEventTypeCustom:
            switch (event.event) {

                case MainMenuSceneReadmeEvent:
                    scene_manager_next_scene(app->scene_manager, ReadmeScene);
                    consumed = true;
                    break;

                case MainMenuSceneQrCodeEvent:
                    scene_manager_next_scene(app->scene_manager, QrCodeInputScene);
                    consumed = true;
                    break;
            }
            break;

        default:
            break;
    }

    return consumed;
}

void main_menu_scene_on_exit(void *context) {
    App *app = context;
    submenu_reset(app->submenu);
}

void text_input_callback(void *context) {
    App *app = context;
    scene_manager_handle_custom_event(app->scene_manager,
            QrCodeInputSceneSaveEvent);
}
void qrcode_input_scene_on_enter(void *context) {
    App *app = context;
    bool clear_text = true;
    text_input_reset(app->text_input);
    text_input_set_header_text(app->text_input, "Enter text");
    text_input_set_result_callback(app->text_input, text_input_callback, app,
            app->qrcode_text, app->qrcode_text_size,
            clear_text);
    view_dispatcher_switch_to_view(app->view_dispatcher, QrCodeTextInputView);
}
bool qrcode_greeting_input_scene_on_event(void *context, SceneManagerEvent event) {
    App *app = context;
    bool consumed = false;
    if (event.type == SceneManagerEventTypeCustom) {
        if (event.event == QrCodeInputSceneSaveEvent) {
            scene_manager_next_scene(app->scene_manager, QrCodeMessageScene);
            consumed = true;
        }
    }
    return consumed;
}
void qrcode_greeting_input_scene_on_exit(void *context) { 
    UNUSED(context); 
}

void generate_qrcode_on_enter(void *context) {
    // Aca deberiamos transformar el texto a qrcode
    App *app = context;
    
    text_box_reset(app->text_box);
    text_box_set_text(app->text_box, app->qrcode_text);
    // TODO: Update when correct QRCode generation function exists...
    //text_box_set_text(app->text_box, qrcode(app->qrcode_text));
    
    view_dispatcher_switch_to_view(app->view_dispatcher, QrCodeTextBoxView);
}

bool qrcode_greeting_message_scene_on_event(void *context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);

    return false; // event not handled.
}
void qrcode_greeting_message_scene_on_exit(void *context) {
    App *app = context;
    widget_reset(app->widget);
}

void readme_scene_on_enter(void *context) {
    App *app = context;
    
    text_box_reset(app->text_box);
    text_box_set_text(
            app->text_box,
            "QRCode Generator is a simple application to create QRCodes\n\n"
            "github.com/qw3rtty/flipperzero_qrcode_generator");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, QrCodeTextBoxView);
}

bool readme_scene_on_event(void *context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    
    return false; // event not handled.
}

void readme_scene_on_exit(void *context) {
    App *app = context;
    submenu_reset(app->submenu);
}

// Arrays for the handlers

void (*const qrcode_scene_on_enter_handlers[])(void *) = {
    qrcode_main_menu_scene_on_enter,
    qrcode_input_scene_on_enter,
    generate_qrcode_on_enter,
    readme_scene_on_enter,
};

bool (*const qrcode_scene_on_event_handlers[])(void *, SceneManagerEvent) = {
    qrcode_main_menu_scene_on_event,
    qrcode_greeting_input_scene_on_event,
    qrcode_greeting_message_scene_on_event,
    readme_scene_on_event,
};

void (*const qrcode_scene_on_exit_handlers[])(void *) = {
    main_menu_scene_on_exit,
    qrcode_greeting_input_scene_on_exit,
    qrcode_greeting_message_scene_on_exit,
    readme_scene_on_exit,
};

static const SceneManagerHandlers qrcode_scene_manager_handlers = {
    .on_enter_handlers = qrcode_scene_on_enter_handlers,
    .on_event_handlers = qrcode_scene_on_event_handlers,
    .on_exit_handlers = qrcode_scene_on_exit_handlers,
    .scene_num = QrCodeSceneCount,
};

// This function is called when a custom event happens
// This event can be triggered when some pin is connected, a timer, etc
static bool basic_scene_custom_callback(void *context, uint32_t custom_event) {
    furi_assert(context);
    App *app = context;
    
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

// This is the function for the back button

bool basic_scene_back_event_callback(void *context) {
    furi_assert(context);
    App *app = context;
    
    return scene_manager_handle_back_event(app->scene_manager);
}

// Alloc for our app
// This is for allocate the memory of our app
static App *app_alloc() {
    App *app = malloc(sizeof(App));

    app->qrcode_text_size = 128;
    app->qrcode_text = malloc(app->qrcode_text_size);

    app->scene_manager = scene_manager_alloc(&qrcode_scene_manager_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher,
            basic_scene_custom_callback);
    view_dispatcher_set_navigation_event_callback(
            app->view_dispatcher, basic_scene_back_event_callback);

    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, QrCodeSubmenuView,
            submenu_get_view(app->submenu));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, QrCodeWidgetView,
            widget_get_view(app->widget));

    app->text_input = text_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, QrCodeTextInputView,
            text_input_get_view(app->text_input));

    app->text_box = text_box_alloc();
    view_dispatcher_add_view(app->view_dispatcher, QrCodeTextBoxView,
            text_box_get_view(app->text_box));
    return app;
}

// For free the memory of the app
static void app_free(App *app) {
    furi_assert(app);

    view_dispatcher_remove_view(app->view_dispatcher, QrCodeSubmenuView);
    view_dispatcher_remove_view(app->view_dispatcher, QrCodeWidgetView);
    view_dispatcher_remove_view(app->view_dispatcher, QrCodeTextInputView);
    view_dispatcher_remove_view(app->view_dispatcher, QrCodeTextBoxView);

    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    submenu_free(app->submenu);
    widget_free(app->widget);
    text_input_free(app->text_input);
    text_box_free(app->text_box);

    free(app);
}

int32_t qrcode_generator_app(void *p) {
    UNUSED(p);
    App *app = app_alloc();

    Gui *gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui,
            ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, MainMenuScene);
    view_dispatcher_run(app->view_dispatcher);

    app_free(app);
    return 0;
}
