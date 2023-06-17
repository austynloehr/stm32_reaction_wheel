import numpy as np

# Wave parameters
f = 1  # Hz
phi1 = 10 # Rad
phi2 = 20 # Rad
phi3 = 30 # Rad

# Create sin waves
w = f * 2 * np.pi
t = np.arange(0, 10, .01)
data1 = np.sin(w*t + phi1)
data2 = np.sin(w*t + phi2)
data3 = np.sin(w*t + phi3)

# Log to file
log = open('Logs/testLog.log', 'w')
for i in range(1,1000):
    string = f"AX: {data1[i]},AY: {data2[i]},AZ: {data3[i]},WX: {data1[i]},WY: {data2[i]},WZ: {data3[i]}\n"
    log.write(string)