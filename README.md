# Firmware-HealthHub
The firmware for the Nordic nrf52832 Bluetooth 5.2 SOC. This is contained in the FCC certified Fanstel BT832 MOC.
It will transport ECG data to David Aldon's [app](https://github.com/DaveAldon/Health-Hub-Mobile).
## Development
The Nordic toolchain and the SEGGER embedded studio were used to develop this. 
### CMSIS
The CMSIS configuration wizard must first be enabled in the SEGGER embedded studio. The following link provides an overview of the process: [Enable CMSIS in SEGGER](http://blog.martincowen.me.uk/adding-cmsis-config-wizard-to-ses-projects.html)
* Java must be installed for it to work.
