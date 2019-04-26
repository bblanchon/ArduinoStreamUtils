# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror)
endif()

include_directories(include)

add_executable(StreamUtilsTest
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

	include/Client.h
	include/Print.h
	include/Stream.h

	doctest.h
	main.cpp
)

add_test(StreamUtils StreamUtilsTest)
