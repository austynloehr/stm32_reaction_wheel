import numpy as np
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import kalman_filter
from ReadBinaryLog import convert_log
import kalmanFunctions as fc
import matplotlib.pylab as plt

logPath = 'Logs/MotorAngleTest.log'

# Struct format for log
lengths = [4,4,4,4]
format = 'Ifff'
labels = ['t','Ax','Ay','Wz']

df = convert_log(format,lengths,labels,logPath)

rawAccelAngle = fc.CalcAccelAngle(df['Ax'], df['Ay'])
rawGyroAngle = fc.CalcGyroAngle(df['t'],df['Wz'],45)

ax1 = plt.plot(df['t'], rawAccelAngle)
ax1 = plt.plot(df['t'], rawGyroAngle)

ax2 = plt.plot(df['t'], df['Ax'])
plt.show()