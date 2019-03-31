# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_compile_options(-Wall -Wextra -Werror)

add_executable(StreamUtilsTest
	BufferedStreamTest.cpp
)

add_test(StreamUtils StreamUtilsTest)
