# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2022
# MIT License

add_library(Stm32Core
	Client.h
	EEPROM.cpp
	EEPROM.h
	Print.h
	Stream.h
)

target_include_directories(Stm32Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/../avr
)

target_compile_definitions(Stm32Core
	PUBLIC
		ARDUINO_ARCH_STM32
)
