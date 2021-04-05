# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2021
# MIT License

add_library(TeensyCore
	Client.h
	EEPROM.cpp
	EEPROM.h
	Print.h
	Stream.h
	WString.h
)

target_include_directories(TeensyCore
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(TeensyCore 
	PUBLIC
		CORE_TEENSY
)
