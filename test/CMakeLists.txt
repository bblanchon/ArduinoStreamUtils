# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCES
	BufferingPrintTest.cpp
	FailingAllocator.hpp
	LoggingPrintTest.cpp
	LoggingStreamTest.cpp
	MemoryStreamTest.cpp
	ReadBufferingStreamTest.cpp
	ReadLoggingStreamTest.cpp
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