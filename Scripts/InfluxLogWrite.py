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
measurement = "DataLogTest"
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
lengths = [4,4,4,4,4,4,4,4,4,4,4,1,1,1]
format = 'IfffffffffiBBB'
labels = ['t','Ax','Ay','Wz','AxFilt','AyFilt','RollAng','RollRate','RollAngCov','RollRateCov','MotorSpeedReq','StateReq','CurrentState','MotorEnable']

# Convert struct
df = convert_log(format, lengths, labels, path)
plt.plot(df.index, df['t'].diff())
plt.show()

theta_accel = CalcAccelAngle(df['AxFilt'], df['AyFilt'])
theta_gyro = CalcGyroAngle(df['t'],df['Wz'],theta_accel[0])
print(theta_accel[0])

# Get current time in ms
t0 = round(time.time_ns() / int(1e6)) 

num_rows = len(df.index)
for idx, row in df.iterrows():
    timestamp  = t0 + int(row["t"])
    # Create influx data points
  
    p1 = (
        Point('IMU')
            .field("Ax",row["Ax"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
   
    p2 = (
        Point('IMU')
            .field("Ay",row["Ay"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
   
    p3 = (
        Point('IMU')
            .field("Wz",row["Wz"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p4 = (
        Point('Status')
            .field("StateReq",row["StateReq"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p5 = (
        Point('Status')
            .field("CurrentState",row["CurrentState"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )

    p6 = (
        Point('Status')
            .field("MotorEnable",row["MotorEnable"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p7 = (
        Point('Balance')
            .field("MotorSpeedReq",row["MotorSpeedReq"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p8 = (
        Point('IMU')
            .field("AxFilt",row["AxFilt"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
   
    p9 = (
        Point('IMU')
            .field("AyFilt",row["AyFilt"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p10 = (
        Point('KalmanFilter')
            .field("RollAng",row["RollAng"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p11 = (
        Point('KalmanFilter')
            .field("RollRate",row["RollRate"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )

    p12 = (
        Point('KalmanFilter')
            .field("RollAngCov",row["RollAngCov"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p13 = (
        Point('KalmanFilter')
            .field("RollRateCov",row["RollRateCov"]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p14 = (
        Point('IMU')
            .field("AccelRollAng",theta_accel[idx]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
    
    p15 = (
        Point('IMU')
            .field("GyroRollAng",theta_gyro[idx]) \
            .time(timestamp,write_precision=WritePrecision.MS)
    )
   
    # Write data to influx
    if write_bool:
        print("Writing data to database...")
        write_api.write(bucket=bucket, org=org, record=p1)
        write_api.write(bucket=bucket, org=org, record=p2)
        write_api.write(bucket=bucket, org=org, record=p3)
        write_api.write(bucket=bucket, org=org, record=p4)
        write_api.write(bucket=bucket, org=org, record=p5)
        write_api.write(bucket=bucket, org=org, record=p6)
        write_api.write(bucket=bucket, org=org, record=p7)
        write_api.write(bucket=bucket, org=org, record=p8)
        write_api.write(bucket=bucket, org=org, record=p9)
        write_api.write(bucket=bucket, org=org, record=p10)
        write_api.write(bucket=bucket, org=org, record=p11)
        write_api.write(bucket=bucket, org=org, record=p12)
        write_api.write(bucket=bucket, org=org, record=p13)
        write_api.write(bucket=bucket, org=org, record=p14)
        write_api.write(bucket=bucket, org=org, record=p15)
        
        pct_complete = (idx / num_rows) * 100
        print(f"{pct_complete: .1f}% Complete")
        
