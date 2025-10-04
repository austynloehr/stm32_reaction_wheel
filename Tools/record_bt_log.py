import serial
import os
import argparse
import sys
import pandas as pd
import time
from lib.bl_frame_handler import BLFrameHandler

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_DIR = os.path.dirname(SCRIPT_DIR)
LOG_DIR = os.path.join(REPO_DIR, 'Logs')
LOG_NAME = 'log' + time.strftime('_%Y%m%d_%H%M%S')
LOG_PATH = os.path.join(LOG_DIR, LOG_NAME + '.parquet')
BL_CONFIG_PATH = 'Tools/bl_frames.yaml'

def main():
    global LOG_PATH
    global BL_CONFIG_PATH
    
    parser = argparse.ArgumentParser(description='Read from HC-05 serial and log frames')
    parser.add_argument('--port', '-p', default='/dev/cu.HC-05')
    parser.add_argument('--baud', '-b', type=int, default=460800)
    parser.add_argument('--log_path', '-o', type=str, default=LOG_PATH)
    args = parser.parse_args()
    port = args.port
    baud = args.baud
    LOG_PATH = args.log_path

    # Initialize frame handler
    frame_handler = BLFrameHandler(BL_CONFIG_PATH)

    # Get the log_frame instance
    log_frame = frame_handler.get_frame('log_frame')

    if log_frame is None:
        raise ValueError(f"log_frame definition not found in {BL_CONFIG_PATH}")

    # Attempt to open serial port
    try:
        ser = serial.Serial(port, baud, timeout=1)
    except Exception as e:
        print(f'Failed to open serial port {port}: {e}')
        sys.exit(2)

    # Begin reading and parsing data
    try:
        data_array = []
        buf = b''
        total_bytes = log_frame.total_length
        cnt = 0
        while True:
            data = ser.read(256)
            if not data:
                continue
            buf += data
            while len(buf) >= total_bytes:
                # Extract one full frame
                frame = buf[:total_bytes]
                
                # Remove extracted frame from buffer
                buf = buf[total_bytes:]

                # Parse frame
                parsed = log_frame.unpack(frame)
                if parsed:
                    cnt += 1
                    print(f'Frames Recieved: {cnt}', end='\r')
                data_array.append(parsed)
    except KeyboardInterrupt:
        print('\n\nExiting…')
        print('May be prompted for sudo password to hang up bluetooth.')
    finally:
        ser.close()

        print(f'\nCollected {len(data_array)} frames.')
        df = pd.DataFrame(data_array)
        df.to_parquet(LOG_PATH, index=True)
        print(f'Wrote log to {LOG_PATH}')
        
        # This tells macOS to hang up
        os.system('sudo pkill -f bluetooth')

if __name__ == '__main__':
    main()
