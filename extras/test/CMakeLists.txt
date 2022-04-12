# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2022
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror -Wundef)
	add_link_options(-static)
endif()

set(SOURCES
	BufferingPrintTest.cpp
	EepromStreamTest.cpp
	FailingAllocator.hpp
	HammingClientTest.cpp
	HammingDecodingClientTest.cpp
	HammingDecodingStreamTest.cpp
	HammingEncodingClientTest.cpp
	HammingEncodingStreamTest.cpp
	HammingPrintTest.cpp
	HammingStreamTest.cpp
	LoggingClientTest.cpp
	LoggingPrintTest.cpp
	LoggingStreamTest.cpp
	MemoryStreamTest.cpp
	ReadBufferingClientTest.cpp
	ReadBufferingStreamTest.cpp
	ReadLoggingClientTest.cpp
	ReadLoggingStreamTest.cpp
	ReadThrottlingStreamTest.cpp
	StringPrintTest.cpp
	StringStreamTest.cpp
	WaitingPrintTest.cpp
	WriteBufferingClientTest.cpp
	WriteBufferingStreamTest.cpp
	WriteLoggingClientTest.cpp
	WriteLoggingStreamTest.cpp
	WriteWaitingClientTest.cpp
	WriteWaitingStreamTest.cpp

	doctest.h
	main.cpp
)

########## AVR ##########

add_subdirectory(cores/avr)

add_executable(StreamUtilsTestAvr ${SOURCES})
target_link_libraries(StreamUtilsTestAvr AvrCore)

add_test(Avr StreamUtilsTestAvr)

########## ESP32 ##########

add_subdirectory(cores/esp32)

add_executable(StreamUtilsTestEsp32 ${SOURCES})
target_link_libraries(StreamUtilsTestEsp32 Esp32Core)

add_test(Esp32 StreamUtilsTestEsp32)

########## ESP8266 ##########

add_subdirectory(cores/esp8266)

add_executable(StreamUtilsTestEsp8266 ${SOURCES})
target_link_libraries(StreamUtilsTestEsp8266 Esp8266Core)

add_test(Esp8266 StreamUtilsTestEsp8266)

########## NRF52 ##########

add_subdirectory(cores/nrf52)

add_executable(StreamUtilsTestNrf52 ${SOURCES})
target_link_libraries(StreamUtilsTestNrf52 Nrf52Core)

add_test(Nrf52 StreamUtilsTestNrf52)

########## SAMD ##########

add_subdirectory(cores/samd)

add_executable(StreamUtilsTestSamd ${SOURCES})
target_link_libraries(StreamUtilsTestSamd SamdCore)

add_test(Samd StreamUtilsTestSamd)

########## STM32 (Official core) ##########

add_subdirectory(cores/stm32)

add_executable(StreamUtilsTestStm32 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32 Stm32Core)

add_test(Stm32 StreamUtilsTestStm32)

########## STM32F1 (Roger's core) ##########

add_subdirectory(cores/stm32f1)

add_executable(StreamUtilsTestStm32F1 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32F1 Stm32F1Core)

add_test(Stm32F1 StreamUtilsTestStm32F1)

########## STM32F4 (Roger's core) ##########

add_subdirectory(cores/stm32f4)

add_executable(StreamUtilsTestStm32F4 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32F4 Stm32F4Core)

add_test(Stm32F4 StreamUtilsTestStm32F4)

########## Teensy ##########

add_subdirectory(cores/teensy)

add_executable(StreamUtilsTestTeensy ${SOURCES})
target_link_libraries(StreamUtilsTestTeensy TeensyCore)

add_test(Teensy StreamUtilsTestTeensy)