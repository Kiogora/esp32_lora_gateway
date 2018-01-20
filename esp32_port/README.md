
ESP32 lora gateway project
==========================

## Testing

Two types of tests are included:

1. Normal tests(Non interactive).
2. Command-line tests(Interactive).

### Normal tests

These tests are executed as unit tests using the unity testing framework. The unity checks are not strictly implemented but are given as an option for the user to choose. The tests run in a menu.

To run the tests, add a test folder as per component as per unit testing documentation in esp-idf, some tests of loragw are already implemented.

To run the tests:


```shell
sh run_unit_tests.sh
```

This will compile and flash the binary to a connected esp32. A concentrator needs to be connected to the spi bus of the device using the pins hardcoded in loragw_spi.

###Command-line tests

This use the linenoise and arg3table library provided within the esp-idf framework.

These tests:

```shell
sh run_cmd_tests.sh
```
