from ReadBinaryLog import convert_log
import time
from influxdb_client import Point, InfluxDBClient, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
import sys
import os

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
lengths = [4,4,4,4]
format = 'Ifff'
labels = ['t','Ax','Ay','Wz']

# Convert struct
df = convert_log(format, lengths, labels, path)

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
   
    # Write data to influx
    if write_bool:
        print("Writing data to database...")
        write_api.write(bucket=bucket, org=org, record=p1)
        write_api.write(bucket=bucket, org=org, record=p2)
        write_api.write(bucket=bucket, org=org, record=p3)
        
        pct_complete = (idx / num_rows) * 100
        print(f"{pct_complete: .1f}% Complete")
        
