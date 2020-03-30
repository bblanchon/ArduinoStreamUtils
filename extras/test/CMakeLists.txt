# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2020
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror -Wundef)
endif()

set(SOURCES
	BufferingPrintTest.cpp
	EepromStreamTest.cpp
	FailingAllocator.hpp
	LoggingClientTest.cpp
	LoggingPrintTest.cpp
	LoggingStreamTest.cpp
	MemoryStreamTest.cpp
	ReadBufferingClientTest.cpp
	ReadBufferingStreamTest.cpp
	ReadLoggingClientTest.cpp
	ReadLoggingStreamTest.cpp
	StringPrintTest.cpp
	StringStreamTest.cpp
	WriteBufferingClientTest.cpp
	WriteBufferingStreamTest.cpp
	WriteLoggingClientTest.cpp
	WriteLoggingStreamTest.cpp

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

########## SAMD ##########

add_subdirectory(cores/samd)

add_executable(StreamUtilsTestSamd ${SOURCES})
target_link_libraries(StreamUtilsTestSamd SamdCore)

add_test(Samd StreamUtilsTestSamd)

########## Teensy ##########

add_subdirectory(cores/teensy)

add_executable(StreamUtilsTestTeensy ${SOURCES})
target_link_libraries(StreamUtilsTestTeensy TeensyCore)

add_test(Teensy StreamUtilsTestTeensy)