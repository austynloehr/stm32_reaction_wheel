import serial
import os
import argparse
import sys
from lib.bl_frame_handler import BLFrameHandler

BL_CONFIG_PATH = 'Tools/bl_frames.yaml'

def main():
    parser = argparse.ArgumentParser(description='Read from HC-05 serial and log frames')
    parser.add_argument('--port', '-p', default='/dev/cu.HC-05')
    parser.add_argument('--baud', '-b', type=int, default=460800)
    args = parser.parse_args()
    port = args.port
    baud = args.baud

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
        buf = b''
        total_bytes = log_frame.total_length
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
                print(parsed)
    except KeyboardInterrupt:
        print('Exiting…')
    finally:
        ser.close()
        # This tells macOS to hang up
        os.system('sudo pkill -f bluetooth')
        print('Serial port closed cleanly.')

if __name__ == '__main__':
    main()
