<div align="center">

<img src="support/epicecu-eeprom-fake-logo.png" alt="EpicECU EEPROMFake" width="400" />

##### An Arduino EEPROM Fake/Mock Library  

</div>

[![Tests](https://github.com/epicecu/eeprom-fake/actions/workflows/unit_tests.yml/badge.svg?branch=main)](https://github.com/epicecu/eeprom-fake/actions/workflows/unit_tests.yml)

## Information

Used for mocking the Arduino EEPROM library in Arduino based unit tests.

Simply include the library under the native build option and test.

Add the following line to your test `#define ENABLE_STRING_SUPPORT` if you require the use of the real Arduino String class for the get/put functions.

## Exmaple

```
[env:native]
platform = native
lib_compat_mode = off
lib_deps =
    ArduinoFake
    git@github.com:epicecu/eeprom-fake.git
```

## Maintainers

- [David Cedar](https://github.com/devvid)

## License

GPL V2 © [Programmor](https://github.com/epicecu/table)
