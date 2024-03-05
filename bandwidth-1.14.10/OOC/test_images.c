
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "Image.h"
#include "MutableImage.h"
#include "Utility.h"
#include "FontPK.h"
#include "colors.h"

static unsigned totalPixelsOfColor (MutableImage *mut, RGB color)
{
	int width = $(mut, width);
	int height = $(mut, height);

	unsigned n=0;

	for (int i= -20; i < width + 20; i++) {
		for (int j= -20; j < height + 20; j++) {
			RGB foundColor = $(mut, pixelAt, i, j);
			if (foundColor == color) 
				n++;
		}
	}
	return n;
}

int main()
{
	Image *img = NULL;
	MutableImage *mut = NULL;

	img = new(Image);
	if ($(img, width) != 0)
		return 1;
	if ($(img, height) != 0)
		return 2;
	release(img);
	if (img != NULL)
		return 3;

	mut = MutableImage_withSize (123, 456);
	if ($(mut, width) != 123)
		return 4;
	if ($(mut, height) != 456)
		return 5;
	release(mut);

	img = Image_fromFile("MarsRover.jpg");
	if (img) {
		if ($(img, width) != 1300)
			return 6;
		if ($(img, height) != 1300)
			return 7;
#define BMP_FILEPATH "/tmp/foo.bmp"
		if (!$(img, writeBMP, BMP_FILEPATH))
			return 8;
		if (!file_exists(BMP_FILEPATH))
			return 9;
		if (0 == get_file_size(BMP_FILEPATH))
			return 10;
		unlink(BMP_FILEPATH);
		release(img);
		if (img != NULL)
			return 11;
	}

	mut = new(MutableImage);
	if ($(mut, width) != 0)
		return 12;
	if ($(mut, height) != 0)
		return 13;
	$(mut, clear);
	$(mut, resize, 1920, 1080);
	if ($(mut, width) != 1920)
		return 14;
	if ($(mut, height) != 1080)
		return 15;

	$(mut, fillWithColor, RGB_CYAN);
	$(mut, drawLine, 0, 0, 1919, 1079, RGB_RED);
	$(mut, drawLine, 0, 0, 0, 1079, RGB_RED);
	unsigned nCyan = totalPixelsOfColor (mut, RGB_CYAN);
	unsigned nRed = totalPixelsOfColor (mut, RGB_RED);
	unsigned total = nCyan + nRed;
	if (total != 1920*1080)
		return 16;
	if (nRed != 1920 + 1080 - 1)
		return 17;

	$(mut, clear);
	unsigned nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080)
		return 18;

	$(mut, fillRectangle, 10, 10, 57, 91, RGB_BLUE);
	unsigned nBlue = totalPixelsOfColor (mut, RGB_BLUE);
	if (nBlue != 57*91)
		return 19;

	$(mut, clear);
	$(mut, fillRectangle, -30, -30, 30, 30, RGB_RED);
	$(mut, fillRectangle, 1920, 30, 30, 30, RGB_RED);
	$(mut, fillRectangle, 33, -30, 30, 30, RGB_RED);
	$(mut, fillRectangle, 550, 1080, 30, 30, RGB_RED);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080)
		return 20;

	$(mut, drawRect, Rect_new(-30, -30, 30, 30), RGB_RED);
	$(mut, drawRectangle, 1920, 30, 30, 30, RGB_RED);
	$(mut, drawRectangle, 33, -30, 30, 30, RGB_RED);
	$(mut, drawRectangle, 550, 1080, 30, 30, RGB_RED);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080)
		return 21;

	$(mut, drawRectangle, -29, -29, 30, 30, RGB_RED);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080 - 1)
		return 22;

	$(mut, putPixel, 0, 0, RGB_WHITE);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080)
		return 23;

	$(mut, drawLine, 0, 0, 1919, 1079, RGB_GREEN);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 1920 * 1080 - 1920)
		return 24;

	RGB color = $(mut, pixelAt, 0, 0);
	if (color != RGB_GREEN)
		return 25;

	$(mut, putPixel, 0, 0, RGB_RED);
	color = $(mut, pixelAt, 0, 0);
	if (color != RGB_RED)
		return 26;

	$(mut, putPixel, 0, 0, RGB_BLUE);
	color = $(mut, pixelAt, 0, 0);
	if (color != RGB_BLUE)
		return 27;

	$(mut, clear);
	color = $(mut, pixelAt, 0, 0);
	if (color != RGB_WHITE)
		return 28;

	release(mut);
	if (mut != NULL)
		return 29;

	mut = MutableImage_withSize (0, 456);
	if ($(mut, width) != 0)
		return 30;
	if ($(mut, height) != 0)
		return 31;
	$(mut, resize, 320, 200);
	if ($(mut, width) != 320)
		return 32;
	if ($(mut, height) != 200)
		return 33;
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite != 320 * 200)
		return 34;

	String *s = _String("this is a test.");
	FontPK *font = FontPK_with(NULL, 0);
	$(mut, drawString, s, 20, 20, (Font*)font, RGB_BLACK);
	nWhite = totalPixelsOfColor (mut, RGB_WHITE);
	if (nWhite == 320 * 200)
		return 35;

	release(mut);

	deallocateClasses();
	return 0;
}

