#!/bin/bash
# After the HC05 module is paired, this script binds the device to rfcomm0 and opens a serial connection
# Pass in argument -l to log to influx database

# Default values:
opt_l=false
opt_d='no value given'

# It's the : after d that signifies that it takes an option argument.

while getopts ld: opt; do
    case $opt in
        l) opt_l=true ;;
        d) opt_d=$OPTARG ;;
        *) echo 'error in command line parsing' >&2
           exit 1
    esac
done

shift "$(( OPTIND - 1 ))"

# Command line parsing is done now.

# HC-05 Mac Address
macAddr='98:D3:02:96:C1:16'

# HC-05 UART baud rate
baudRate=460800

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
sudo minicom -H -D /dev/rfcomm0 -b $baudRate -C $logFile

# Relase rfcomm0 once minicom is closed
sudo rfcomm release 0 

# --------------------------------------------------
# Change this code to work without virtualenvwrapper
# Activate venv
source virtualenvwrapper.sh
workon ReactionWheelTools

# Call python script
if $opt_l; then
    python -B Scripts/InfluxLogWrite.py $logFile
fi

# Deactivate venv
deactivate
# --------------------------------------------------
