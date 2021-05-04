# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2021
# MIT License

add_library(Nrf52Core INTERFACE)

target_include_directories(Nrf52Core
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Nrf52Core
	INTERFACE
		ARDUINO_ARCH_NRF52
)
