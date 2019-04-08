# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(StreamUtilsTest
	FailingAllocator.hpp
	Stream.hpp
	StreamSpy.hpp
	StreamStub.hpp
	StreamWithInputBufferTest.cpp
	doctest.h
	main.cpp
)

add_test(StreamUtils StreamUtilsTest)
