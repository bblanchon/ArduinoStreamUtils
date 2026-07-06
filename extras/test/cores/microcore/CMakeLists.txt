# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2026
# MIT License

add_library(MicroCore
	EEPROM.cpp
	EEPROM.h
	Print.h
	Stream.h
)

target_include_directories(MicroCore
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(MicroCore
	PUBLIC
		ARDUINO_attiny
		ARDUINO_ARCH_AVR
)

add_streamutils_test(StreamUtilsTestMicroCore MicroCore)
