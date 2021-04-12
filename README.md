# Firmware-HealthHub
The firmware for the Nordic nrf52832 Bluetooth 5.2 SOC. This is contained in the FCC certified Fanstel BT832 MOC.
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
## Flashing
 - Run the commandline utility JLinkExe
 - type "connect"
 - Specify the device "NRF52832_XXAA" (It should be the default, pressing enter selects the default)
 - Specify target interface "SWD" with "S"
 - Hit enter for default target interface speed
 - type "loadfile" followed by the path to the hex or bin file to flash
