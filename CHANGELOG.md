StreamUtils - Changelog
=======================

1.2.2 (2019/07/18)
-----

* Fix BufferingPrint taking Stream instead of Print (issue #3)
* Fix LoggingPrint not forwarding call to Print::flush()
* Fix missing `override` specifiers

1.2.1 (2019/06/05)
-----

* Remove workaround for ESP8266 core 2.5.0
* Fix compatibility with ESP8266 core 2.5.1+ (issue #2)

1.2.0 (2019/05/01)
-----

* Add `LoggingPrint`
* Add `BufferingPrint`
* Add `WriteLoggingClient`, `ReadLoggingClient`, and `LoggingClient`
* Add `WriteBufferingClient` and `ReadBufferingClient`

1.1.0 (2019/04/20)
-----

* Add `LoggingStream` (=`ReadLoggingStream` + `WriteLoggingStream`)

1.0.0 (2019/04/14)
-----

* Add `ReadBufferingStream` and `WriteBufferingStream`
* Add `ReadLoggingStream` and `WriteLoggingStream`
