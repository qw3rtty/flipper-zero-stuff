#include "qrcode_renderer.h"
#include "qrencode/qrcodegen.h"
#include <gui/canvas.h>

void render_qrcode(Canvas* canvas, const uint8_t* qrcode) {
	int size = qrcodegen_getSize(qrcode);
	const int scale = 2;
	const int offset_x = 64 - (size * scale) / 2;
	const int offset_y = 32 - (size * scale) / 2;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			if(qrcodegen_getModule(qrcode, x, y)) {
				canvas_draw_box(canvas, offset_x + x * scale, offset_y + y * scale, scale, scale);
			}
		}
	}

    canvas_commit(canvas);
}


static void generate_and_print_qrcode(Canvas* canvas, const char* text) {
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
			qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

    render_qrcode(canvas, qrcode);
}

