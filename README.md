<div align="center">

<img src="support/epicecu-eeprom-fake-logo" alt="EpicECU EEPROMFake" width="400" />

##### An open source Table library supporting 2d and 3d modes. This is useful for lookups e.g. fuel, timing maps.

</div>

[![Tests](https://github.com/epicecu/eeprom-fake/actions/workflows/unit_tests.yml/badge.svg?branch=main)](https://github.com/epicecu/eeprom-fake/actions/workflows/unit_tests.yml)

## Information

Used for moking the Arduino EEPROM library in Arduino based unit tests.

Simply include the library under the native build option and test.

Add the line to your test `#define ENABLE_STRING_SUPPORT` if you require the use of the real Arduino String class for the get/put functions.

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
