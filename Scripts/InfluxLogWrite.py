from ReadBinaryLog import convert_log
import time
from influxdb_client import Point, InfluxDBClient, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from kalmanFunctions import CalcAccelAngle, CalcGyroAngle
import sys
import os
from matplotlib import pyplot as plt

path = sys.argv[1]

# Raise error if file does not exist
if not os.path.isfile(path):
    raise FileNotFoundError

write_bool = True
check_sample_time = False

bucket = "reaction_wheel"
org = "dev"

# API setup
token = "oamovLGD2QzIY6YG2vaIgbNcN0MbY57Xb_GOm4U3LVy5A6nQrquVUCtM8ibsk3b8Rm4-9mJ8Yl_o2r7PHQzdKg=="
url="http://localhost:8086"
client = InfluxDBClient(
   url=url,
   token=token,
   org=org
)
write_api = client.write_api(write_options=SYNCHRONOUS)

# Struct format
lengths = [4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1]
format = 'IfffffffiiiiiiiiBBBB'
labels = ['t',
          'Ax','Ay','Wz','AxFilt','AyFilt',
          'RollAng','RollRate',
          'MotorSpeed','MotorCurrent','MotorRequest','PIDTerm','MotorSpeedTerm','pTerm','iTerm','dTerm',
          'MotoControlMode','StateReq','CurrentState','MotorEnable']
categories = ['IMU','IMU','IMU','IMU','IMU',
            'KalmanFilter','KalmanFilter',
            'Balance','Balance','Balance','Balance','Balance','Balance','Balance','Balance',
            'Status','Status','Status','Status']

# Convert struct
df = convert_log(format, lengths, labels, path)

if check_sample_time:
    plt.plot(df.index, df['t'].diff())
    plt.show()

# theta_accel = CalcAccelAngle(df['AxFilt'], df['AyFilt'])
# theta_gyro = CalcGyroAngle(df['t'],df['Wz'],theta_accel[0])

# Get current time in ms
t0 = round(time.time_ns() / int(1e6)) 

num_rows = len(df.index)
for i, row in df.iterrows():
    timestamp  = t0 + int(row["t"])
    for j, cat in enumerate(categories):
        # Create influx data points
        label = labels[j+1]  # Skip t

        p = (
            Point(cat)
                .field(label,row[label]) \
                .time(timestamp,write_precision=WritePrecision.MS)
        )
                
        # Write data to influx
        if write_bool:
            print("Writing data to database...")
            write_api.write(bucket=bucket, org=org, record=p)
            
            pct_complete = (i / num_rows) * 100
            print(f"{pct_complete: .1f}% Complete")