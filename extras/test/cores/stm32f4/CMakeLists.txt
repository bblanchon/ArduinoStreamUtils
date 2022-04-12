# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2022
# MIT License

add_library(Stm32F4Core INTERFACE)

target_include_directories(Stm32F4Core
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Stm32F4Core
	INTERFACE
		ARDUINO_ARCH_STM32F4
)
