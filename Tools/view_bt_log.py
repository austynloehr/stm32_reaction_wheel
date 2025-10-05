import os
import argparse
import rerun as rr
import pandas as pd

def main():
    '''Plot Bluetooth log data from parquet file using Rerun.'''

    parser = argparse.ArgumentParser(description='Plot Bluetooth log data from parquet file')
    parser.add_argument('--file', '-f',required=True, help='Path to parquet log file')
    args = parser.parse_args()

    file_path = args.file

    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"File {file_path} does not exist")

    # Import data
    df = pd.read_parquet(file_path)
    
    # Initialize rerun
    rr.init("reaction_wheel_playback", spawn=True)

    # Log all data to Rerun
    for idx, row in df.iterrows():
        rr.set_time("abs_time", timestamp=df['datetime'][idx])
        for col in df.columns:
            if col == 'datetime' or col == 't':
                continue
            rr.log(f"signal/{col}", rr.Scalars(df[col][idx]))

if __name__ == '__main__':
    main()