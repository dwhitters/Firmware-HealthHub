# Firmware-HealthHub
The firmware for the Nordic nrf52832 Bluetooth 5.2 SOC. This is contained in the FCC certified Fanstel BT832 MOC.
It runs on a custom [PCB](https://github.com/dwhitters/KiCad-HealthHub).
It will transport ECG data to David Aldon's [app](https://github.com/DaveAldon/Health-Hub-Mobile).
## Development
The Nordic toolchain and the SEGGER embedded studio were used to develop this. 
https://devzone.nordicsemi.com/nordic/nrf-connect-sdk-guides/b/getting-started/posts/nrf-connect-sdk-tutorial
### Starting Samples
**blinky** and **out_of_tree_board** were used to verify that the GPIOs on the BGA were operation.
### Setup
Use the nRF connect app to setup your system. http://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.2/nrf/gs_assistant.html#gs-app-gsa
Pacman was used successfully to set this up. Some packages were named slightly different and the jlink install had to come separately from the AUR.
### Building
Run **Run CMake** under the **Project** tab on the toolbar after any updates to the devicetree. 
### Troubleshooting
#### Moving examples from the SDK directory
https://devzone.nordicsemi.com/f/nordic-q-a/44638/how-to-move-an-sdk-example-out-of-the-sdk-tree
Remove the KConfig directory and the build_<board> directory, then open the project with "Clean Build" checked.
### Logging
Segger's RTT [Tutorial](https://www.segger.com/products/debug-probes/j-link/tools/rtt-viewer/) outlines how to connect to the device and view the logs. The RTTViewer is easy to use and will be used in stand-alone mode most of the time. 
Nordic's [documentation](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v12.3.0%2Fgroup__nrf__log__config.html&anchor=ga3016ad60bd2f3cd631d8371b2f98b74e) covers the firmware implementation.
## Flashing
 * Target Interface: SWD
 * Device: NRF52832_XXAA
 * Endianness: Little Endian
### JLinkExe
 - type "connect"
 - Specify the device "NRF52832_XXAA" (It should be the default, pressing enter selects the default)
 - Specify target interface "SWD" with "S"
 - Hit enter for default target interface speed
 - type "loadfile" followed by the path to the hex or bin file to flash
 - Remove the debugger cable and power cycle the device.
### JFlash
 - Create new project
 - Specify the device and target interface
 - Target->Connect
 - Flash
