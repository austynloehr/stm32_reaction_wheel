import matplotlib.pyplot as plt
import pandas as pd
import readline
import os.path
import json
import sys


def main():
    userInputReq = len(sys.argv)  < 2

    if userInputReq: 
        readline.set_completer_delims(' \t\n=')  # CL input delimiters
        readline.parse_and_bind("tab: complete") # Call tab complete function
        path = input("Enter log path: ")  # Get user input with file complete
    else:
        path = sys.argv[1]

    # Raise error if file does not exist
    if not os.path.isfile(path):
        raise FileNotFoundError

    # Parse file
    df = parse(path)

    # Plot signals
    plt.plot(df['TK'], df['aRoll'], marker = '.')
    plt.plot(df['TK'], df['aRollFilt'], marker = '.', color='m')
    plt.plot(df['TK'], df['gRoll'], marker = '.', color='g')
    plt.plot(df['TK'], df['compRoll'], marker = '.', color='r')
    plt.grid(True)

    # fig1, axs = plt.subplots(3, 1, sharex=True)

    # axs[0].plot(df.index, df["AX"], marker = '.')
    # axs[0].set_ylabel('Ax (m/s^2)')
    # axs[0].grid(True)

    # axs[1].plot(df.index, df["AY"], marker = '.')
    # axs[1].set_ylabel('Ay (m/s^2)')
    # axs[1].grid(True)

    # axs[2].plot(df.index, df["AZ"], marker = '.')
    # axs[2].set_ylabel('Az (m/s^2)')
    # axs[2].grid(True)

    # fig1.suptitle('Acceleration Data', fontsize=16)
    # fig1.supxlabel('Samples',fontsize=16)

    # fig2, axs = plt.subplots(3, 1, sharex=True)
    # axs[0].plot(df.index, df["WX"], marker = '.')
    # axs[0].set_ylabel('Wx (deg/s)')
    # axs[0].grid(True)

    # axs[1].plot(df.index, df["WY"], marker = '.')
    # axs[1].set_ylabel('Wy (deg/s)')
    # axs[1].grid(True)

    # axs[2].plot(df.index, df["WZ"], marker = '.')
    # axs[2].set_ylabel('Wz (deg/s)')
    # axs[2].grid(True)

    # fig2.suptitle('Gyro Data', fontsize=16)
    # fig2.supxlabel('Samples', fontsize=16)
    
    plt.show()


def parse(path:str) -> pd.DataFrame:
    # This function takes a log file, parses data, and convert it do a pandas dataframe

    log = open(path, 'r').read().splitlines(True)  # Open log file, store each line as an array element

    # Indexes of first and last lines in log, these may be incomplete lines
    firstNdx = 0
    lastNdx= len(log)-1

    result={}  # Placeholder dictionary for all formatted data
    i = 0
    for ndx, line in enumerate(log):
        if ndx not in [firstNdx, lastNdx]:  # Parse and store line if not the first or last line
            columns = line.split(',')  # Split up data 
            data={}  # Placeholder dictionary for individual lines of data
            for c in columns:
                key = c.split(':')[0]  # Extract label
                value = float(c.split(':')[1])  # Extract value
                data[key] = value  # Store to data dictionary
            result[i] = data  # Store completed data dictionary to result dictionary
            i+=1  # Move on to next line of log

    j=json.dumps(result)  # Convert dictionary to json format
    df=pd.read_json(j, orient='index')  # Convert json to pandas dataframe

    return df
    


if __name__=='__main__':
    main()