BOARD=esp8266:esp8266:d1
PORT=/dev/cu.wchusbserial1450

ARDUINO=/Applications/Arduino.app/Contents/MacOS/Arduino

SRCS=wipi.ino 1wire.ino analog.ino serial.ino gpio.ino

all: $(SRCS)
	$(ARDUINO) --board $(BOARD) --verify wipi.ino

install: $(SRCS)
	$(ARDUINO) --board $(BOARD) --port $(PORT) --upload wipi.ino
