/*
 This file is part of rainbowduino_firmware_rigel.

 rainbowduino_firmware_rigel is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 rainbowduino_firmware_rigel is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with rainbowduino_firmware_rigel.  If not, see <http://www.gnu.org/licenses/

 ABOUT THE FIRMWARE
 rainbowduino_firmware_rigel - alternative firmware for colorduino boards produced by ITeadStudio
 Copyright (c) 2011 Vichaya Sidhipong, bsidhipong@gmail.com

*/

#ifndef RAINBOWDUINO_H
#define RAINBOWDUINO_H

/* I2C address scheme:
 * DEVICE [2:0]
 * SLAVE ID [3:0]
 *
 * Rainbowduino (1, 001b)
 * Colorduino (2, 010b) */
#define I2C_DEVICE_ADDRESS 0x10
#define MAX_WIRE_CMD_LEN 48 /* maximum of 92 coordinates can be passed in one packet */

/* NOTE: Hardware output order is GRB */
typedef struct {
	uint8_t red:5;
	uint8_t green:5;
	uint8_t blue:5;
} rgb_triplet_t;

void clear_buffer( void );
void show_buffer( void );
void draw_pixel_rgb( uint8_t col, uint8_t row, uint8_t red, uint8_t green, uint8_t blue );
void draw_pixel( uint8_t col, uint8_t row );

#endif

/* vim: set noexpandtab tabstop=4 shiftwidth=4 nolist autoindent cindent: */
