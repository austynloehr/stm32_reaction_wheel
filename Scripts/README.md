# STM32 Reaction Wheel Scipts

This directory contains scripts used for data logging and visualization.

## Serial Monitor

*Note: This script will likely only work on linux based systems*

This script connects the HC-05 BLE module, prints/logs serial data using minicom, then parses log file and plots using a python script. Data is only plotted after logging session is complete, it is not a real-time plotting tool.

### Setup
#### Virtual Environment Setup

This script was developed using the [VirtualEnvWraper](https://virtualenvwrapper.readthedocs.io/en/latest/). Any python scripts will try to run using a Virtual Environment located in $HOME/.virtualenvs. For initial setup, please refer to the documenation linked above. This [link](https://opensource.com/article/21/2/python-virtualenvwrapper) is also helpful for Ubuntu setup.<br>

*Note: If using an alternative virtual environment is desired, change the highlighted code section within [ble_serial_monitor.sh](ble_serial_monitor.sh)*<br>

Once VirtualEnvWraper has been setup, run the commands below to setup virtual environment and install neccessary python packages.<br>

```
cd ~/stm32_reaction_wheel
mkvirtualenv ReactionWheelTools
workon ReactionWheelTools
pip install -r Scripts/requirements.txt
```
#### Connecting to HC05
Follow steps [here](https://alberand.com/hc-05-linux.html) to connect and pair to HC05 BLE module. Take note of device MAC address. 

#### Install Minicom

Minicom is used to connect to rfcomm0 port and view serial data being sent from HC-05 module.<br>

Install minicom using the commands below.<br>

```
sudo apt update
sudo apt install minicom
```

#### Update HC-05 MAC Address

The mac address for HC-05 module is hardcoded in [ble_serial_monitor.sh](ble_serial_monitor.sh). In order for this to work with a different device, the $macAddr variable must be updated to the MAC address of your specific device.

### Running Script
*Note: This script also be ran from (~/stm32_reaction_wheel)*

Execute the commands below to run the script.

```
cd ~/stm32_reaction_wheel/Scripts
./ble_serial_monitor.sh
```

To terminate logging session hit CTRL+A then X to end minicom session. Data will then be plotted and displayed.