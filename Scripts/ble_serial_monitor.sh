#!/bin/bash
# After the HC05 module is paired, this script binds the device to rfcomm0 and opens a serial connection 

# HC-05 Mac Address
macAddr='38:AB:41:3C:D1:31'

# HC-05 UART baud rate
baudRate=921600

# Bind rfcomm0 if does not exist already
sudo rfcomm bind 0 $macAddr

# Change working directory to ~/STM32_reaction_weel if ran from ~/STM32_reaction_weel/Scripts
if [[ "$PWD" = */Scripts* ]] 
then 
    cd ../
fi

# Create empty log file for logging data
timestamp=$(date +%Y-%m-%d_%T)
logFile="Logs/IMUtesting_$timestamp.log"
touch "${logFile}"

# Begin serial coms
sudo minicom -D /dev/rfcomm0 -b $baudRate -C $logFile

# Relase rfcomm0 once minicom is closed
sudo rfcomm release 0 

# --------------------------------------------------
# Change this code to work without virtualenvwrapper
# Activate venv
source virtualenvwrapper.sh
workon ReactionWheelTools

# Call python script
python -B Scripts/LogPlotter.py $logFile

# Deactivate venv
deactivate
# --------------------------------------------------
