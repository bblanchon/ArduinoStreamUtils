# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror -Wundef)
endif()

set(SOURCES
	BufferingPrintTest.cpp
	FailingAllocator.hpp
	LoggingClientTest.cpp
	LoggingPrintTest.cpp
	LoggingStreamTest.cpp
	MemoryStreamTest.cpp
	ReadBufferingStreamTest.cpp
	ReadBufferingClientTest.cpp
	ReadLoggingClientTest.cpp
	ReadLoggingStreamTest.cpp
	WriteBufferingClientTest.cpp
	WriteBufferingStreamTest.cpp
	WriteLoggingClientTest.cpp
	WriteLoggingStreamTest.cpp

	doctest.h
	main.cpp
)

########## AVR ##########

add_library(AvrCore INTERFACE)
target_include_directories(AvrCore INTERFACE cores/avr/)
target_compile_definitions(AvrCore INTERFACE ARDUINO_ARCH_AVR)

add_executable(StreamUtilsTestAvr ${SOURCES})
target_link_libraries(StreamUtilsTestAvr AvrCore)

add_test(Avr StreamUtilsTestAvr)

########## ESP8266 ##########

add_library(Esp8266Core INTERFACE)
target_include_directories(Esp8266Core INTERFACE cores/esp8266/)
target_compile_definitions(Esp8266Core INTERFACE ARDUINO_ARCH_ESP8266)

add_executable(StreamUtilsTestEsp8266 ${SOURCES})
target_link_libraries(StreamUtilsTestEsp8266 Esp8266Core)

add_test(Esp8266 StreamUtilsTestEsp8266)

########## SAMD ##########

add_library(SamdCore INTERFACE)
target_include_directories(SamdCore INTERFACE cores/samd/)
target_compile_definitions(SamdCore INTERFACE ARDUINO_ARCH_SAMD)

add_executable(StreamUtilsTestSamd ${SOURCES})
target_link_libraries(StreamUtilsTestSamd SamdCore)

add_test(Samd StreamUtilsTestSamd)