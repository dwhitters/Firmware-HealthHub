# Firmware-HealthHub
The firmware for the Nordic nrf52832 Bluetooth 5.2 SOC. This is contained in the FCC certified Fanstel BT832 MOC.
It will transport ECG data to David Aldon's [app](https://github.com/DaveAldon/Health-Hub-Mobile).
## Development
The Nordic toolchain and the SEGGER embedded studio were used to develop this. 
### Setup
Use the nRF connect app to setup your system. http://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.2/nrf/gs_assistant.html#gs-app-gsa
Pacman was used successfully to set this up. Some packages were named slightly different and the jlink install had to come separately from the AUR.
