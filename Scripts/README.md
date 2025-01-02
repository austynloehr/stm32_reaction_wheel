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
cd stm32_reaction_wheel
./Scripts/ble_serial_monitor.sh
```

To terminate logging session hit CTRL+A then X to end minicom session. Raw hex data will be logged to a file located in stm32_reaction_wheel/Logs.

### Converting Raw Data

All data is written as a struct over UART. In order to interpret the data, it must be parsed and unpacked according the proper struct definition. As of initial release, log data is packed in the following order:

```
typedef struct LogPayload{
	uint32_t tick;
	float ax_mps2;
	float ay_mps2;
	float wz_dps;
	float axFilt_mps2;
	float ayFilt_mps2;
	float rollAngle_deg;
	float rollRate_deg;
	int32_t MotorSpeed_rpm;
	int32_t MotorCurrent_mA;
	int32_t MotorRequest_na;
	int32_t PIDTerm_mA;
	int32_t MotorSpeedTerm_mA;
	int32_t pTerm_mA;
	int32_t iTerm_mA;
	int32_t dTerm_mA;
	uint8_t MotorControlMode_enum;
	uint8_t StateReq_enum;
	uint8_t CurrentState_enum;
	uint8_t MotorEnable_bool;
} __attribute__ ((packed)) LogPayload_t;
```
A python script (ReadBinaryLog.py) was created to parse the data according to a user-provided struct. The data is parsed and output as a pandas dataframe. An example of using this script can be found below.

```
from ReadBinaryLog import convert_log

# Log path
path = Logs/exampleLog.log

# Struct format
# Lengths of each variable
lengths = [4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1]

# Type of each variable
format = 'IfffffffiiiiiiiiBBBB'

# Data label for each variable
labels = ['t',
          'Ax','Ay','Wz','AxFilt','AyFilt',
          'RollAng','RollRate',
          'MotorSpeed','MotorCurrent','MotorRequest','PIDTerm','MotorSpeedTerm','pTerm','iTerm','dTerm',
          'MotoControlMode','StateReq','CurrentState','MotorEnable']

# Convert struct
df = convert_log(format, lengths, labels, path);
```

### Influx / Grafana Logging

InfluxLogWrite.py was created to write log data to an Influx database and visualize using Grafana.

1. Setup Influxdb and create a bucket
2. Setup grafana and create a new dashboard
3. Update InfluxLogWrite.py with your Influx API information.
```
bucket = "Your bucket name"
org = "Your org name"

# API setup
token = "Your API Token"
url="http://localhost:8086"
```
4. Call script with path to log file
```
python Scripts/InfluxLogWrite.py Logs/ExampleLog.log
```
