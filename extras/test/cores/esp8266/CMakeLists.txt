# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2021
# MIT License

add_library(Esp8266Core
	Client.h
	EEPROM.cpp
	EEPROM.h
	Print.h
	Stream.h
	WString.h
)

target_include_directories(Esp8266Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Esp8266Core 
	PUBLIC
		ARDUINO_ARCH_ESP8266
)
