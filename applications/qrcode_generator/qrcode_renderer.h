#pragma once

#include <gui/canvas.h>
#include <stdint.h>

void render_qrcode(Canvas* canvas, const uint8_t* qrcode);

static void generate_and_print_qrcode(Canvas* canvas, const char* text);
