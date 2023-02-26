# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2023
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-Wall -Wextra -Werror -Wundef)
	add_link_options(-static)
endif()

file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp)

add_subdirectory(doctest)

#
# AVR
#
add_subdirectory(cores/avr)

add_executable(StreamUtilsTestAvr ${SOURCES})
target_link_libraries(StreamUtilsTestAvr AvrCore doctest)

add_test(Avr StreamUtilsTestAvr)

#
# ESP32
#
add_subdirectory(cores/esp32)

add_executable(StreamUtilsTestEsp32 ${SOURCES})
target_link_libraries(StreamUtilsTestEsp32 Esp32Core doctest)

add_test(Esp32 StreamUtilsTestEsp32)

#
# ESP8266
#
add_subdirectory(cores/esp8266)

add_executable(StreamUtilsTestEsp8266 ${SOURCES})
target_link_libraries(StreamUtilsTestEsp8266 Esp8266Core doctest)

add_test(Esp8266 StreamUtilsTestEsp8266)

#
# NRF52
#
add_subdirectory(cores/nrf52)

add_executable(StreamUtilsTestNrf52 ${SOURCES})
target_link_libraries(StreamUtilsTestNrf52 Nrf52Core doctest)

add_test(Nrf52 StreamUtilsTestNrf52)

#
# SAMD
#
add_subdirectory(cores/samd)

add_executable(StreamUtilsTestSamd ${SOURCES})
target_link_libraries(StreamUtilsTestSamd SamdCore doctest)

add_test(Samd StreamUtilsTestSamd)

#
# STM32 (Official core)
#
add_subdirectory(cores/stm32)

add_executable(StreamUtilsTestStm32 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32 Stm32Core doctest)

add_test(Stm32 StreamUtilsTestStm32)

#
# STM32F1 (Roger's core)
#
add_subdirectory(cores/stm32f1)

add_executable(StreamUtilsTestStm32F1 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32F1 Stm32F1Core doctest)

add_test(Stm32F1 StreamUtilsTestStm32F1)

#
# STM32F4 (Roger's core)
#
add_subdirectory(cores/stm32f4)

add_executable(StreamUtilsTestStm32F4 ${SOURCES})
target_link_libraries(StreamUtilsTestStm32F4 Stm32F4Core doctest)

add_test(Stm32F4 StreamUtilsTestStm32F4)

#
# Teensy
#
add_subdirectory(cores/teensy)

add_executable(StreamUtilsTestTeensy ${SOURCES})
target_link_libraries(StreamUtilsTestTeensy TeensyCore doctest)

add_test(Teensy StreamUtilsTestTeensy)