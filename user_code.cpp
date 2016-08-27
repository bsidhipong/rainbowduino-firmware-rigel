#include <math.h>
#include <stdint.h>
#include <Arduino.h>
#include "rainbowduino.h"

/*
 The plasma effect code is based on lincomatic's "ColorduinoPlasma" demo, for which the copyright header is reproduced below:

 ColorduinoPlasma - Plasma demo using Colorduino Library for Arduino
 Copyright (c) 2011 Sam C. Lin lincomatic@hotmail.com ALL RIGHTS RESERVED

 based on  Color cycling plasma   
 Version 0.1 - 8 July 2009
 Copyright (c) 2009 Ben Combee.  All right reserved.
 Copyright (c) 2009 Ken Corey.  All right reserved.
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.

 The plasma effect code consists of:
 plasma_effect( )
 rgb_to_hsv( ) although this in itself seems to be based on many available implementations on the internet
*/

typedef struct {
	uint8_t h;
	uint8_t s;
	uint8_t v;
} hsv_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_t;

typedef struct {
	uint8_t energy;
	uint8_t inertia;
	uint8_t speed;
	uint8_t heartbeat;
} matrix_cell_t;

float dist ( float a, float b, float c, float d ) 
{
	return sqrt( (c-a)*(c-a)+(d-b)*(d-b) );
}

void hsv_to_rgb( hsv_t *hsv, rgb_t *rgb )
{
	float r, g, b;
	float h = (float)(hsv->h / 256.0);
	float s = (float)(hsv->s / 256.0);
	float v = (float)(hsv->v / 256.0);

	if ( hsv->s == 0 ) { /* achromatic gray */
		r = g = b = v;
	}
	else {
		int16_t i;
		float f, p, q, t;
		h *= 6.0;
		i = (int)(floor(h)); /* integer part of h */
		f = h - i; /* fractional part of h */
		p = (float)(v * (1.0 - s));
		q = (float)(v * (1.0 - (s * f)));
		t = (float)(v * (1.0 - (s * (1.0 - f))));
		switch ( i ) {
			case  0: r = v; g = t; b = p; break;
			case  1: r = q; g = v; b = p; break;
			case  2: r = p; g = v; b = t; break;
			case  3: r = p; g = q; b = v; break;
			case  4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
		}
	}
	rgb->r = (uint8_t)(r * 255.0);
	rgb->g = (uint8_t)(g * 255.0);
	rgb->b = (uint8_t)(b * 255.0);
}

void plasma_effect ( )
{
	static uint32_t palette_shift = 128000;
	hsv_t hsv;
	rgb_t rgb;
	uint8_t row, col;
	for ( row = 0; row < 8; row++ ) {
		for ( col = 0; col < 8; col++ ) {
			float temp;
			temp = sin(dist(col + palette_shift, row, 128.0, 128.0) / 8.0)
				 + sin(dist(col, row, 64.0, 64.0) / 8.0)
				 + sin(dist(col, row + palette_shift / 7, 192.0, 64) / 7.0)
				 + sin(dist(col, row, 192.0, 100.0) / 8.0);
			hsv.h = (uint8_t)((temp) * 128) & 0xFF;
			hsv.s = 255;
			hsv.v = 16;
			hsv_to_rgb( &hsv, &rgb );
			draw_pixel_rgb( col, row, rgb.r, rgb.g, rgb.b );
		}
	}
	show_buffer( );
	palette_shift++;
}

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
matrix_cell_t matrix [MATRIX_HEIGHT+1][MATRIX_WIDTH];
void enter_the_matrix ( void )
{
	uint8_t col, row;
	/* birth */
	col = 0;
	while ( col < MATRIX_WIDTH ) {
		matrix_cell_t *cell = &matrix[0][col];
		if ( cell->energy == 0 ) {
			if ( random( 100 ) % 85 == 0 ) {
				cell->inertia = 8 * (random( 16 )+1);
				cell->speed = random( 48 ) +1; /* cell makes a drop every "speed" frames */
				cell->heartbeat = 0;
				cell->energy = 255;
			}
		}
		col++;
	}
	/* update */
	row = 8;
	do {
		row--;
		col = 0;
		while ( col < MATRIX_WIDTH ) {
			matrix_cell_t *cell = &matrix[row][col];
			/* if this cell is alive... */
			if ( cell->energy ) {
				if ( ++cell->heartbeat == cell->speed ) {
					matrix_cell_t *down = &matrix[row+1][col];
					down->energy = cell->energy;
					down->inertia = cell->inertia;
					down->speed = cell->speed;
					down->heartbeat = cell->heartbeat = 0; /* reset heartbeat once cell drops */
					cell->energy = max( 0, cell->energy - cell->inertia);
				}
			}
			col++;
		}
	} while ( row );
	/* draw output */
	clear_buffer( );
	row = 0;
	while ( row < MATRIX_HEIGHT ) {
		col = 0;
		while ( col < MATRIX_WIDTH ) {
			matrix_cell_t *cell = &matrix[row][col];
			draw_pixel_rgb( col, row, (cell->energy == 255) ? 31 : 0, (cell->energy == 255) ? 31 : (uint8_t) ((cell->energy / 255.0) * 31.0), (cell->energy == 255) ? 31 : 0);
			col++;
		}
		row++;
	}
	show_buffer( );
}

static uint16_t frames = 0;
void do_periodic_task( void )
{
	if ( frames > 5000 ) plasma_effect( );
	else enter_the_matrix( );
	if ( ++frames > 5100 ) frames = 0;
}

/* vim: set noexpandtab tabstop=4 shiftwidth=4 nolist autoindent cindent: */
