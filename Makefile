PROJECT_DIR=$(HOME)/Projects
ARDMK_DIR=$(PROJECT_DIR)/Arduino-Makefile

BOARD_TAG=
BOARD_NAME=uno
ARDUINO_LIBS=
AVRDUDE_ARD_BAUDRATE=57600
MONITOR_PORT=/dev/tty.*usbserial*
MONITOR_BAUDRATE=57600
CPPFLAGS=-Wfatal-errors

include $(ARDMK_DIR)/Arduino.mk
