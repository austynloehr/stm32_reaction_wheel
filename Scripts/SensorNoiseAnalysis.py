import numpy as np
from matplotlib import pyplot as plt
from ReadBinaryLog import convert_log
import kalmanFunctions as kf

logPath = 'Logs/40k_noise.log'

# Struct format
lengths = [4,4,4,4,4,1,1,1]
format = 'IfffiBBB'
labels = ['t','Ax','Ay','Wz','MotorSpeedReq','StateReq','CurrentState','MotorEnable']

df = convert_log(format,lengths,labels,logPath)
ax = df['Ax']
ay = df['Ay']
wz = df['Wz']
t = df['t']

# calculate angles
theta_accel = kf.CalcAccelAngle(ax, ay)
theta_gyro = kf.CalcGyroAngle(t,wz,45)

# mu, std
muAx, stdAx = [np.mean(ax), np.std(ax)]
muAy, stdAy = [np.mean(ay), np.std(ay)]
muWz, stdWz = [np.mean(wz), np.std(wz)]
muThetaAccel, stdThetaAccel = [np.mean(theta_accel), np.std(theta_accel)]
muThetaGyro, stdThetaGyro = [np.mean(theta_gyro), np.std(theta_gyro)]

print(stdAx)
print(stdAy)
print(stdWz)
print(stdThetaAccel)
print(stdThetaGyro)

# Create histograms for stationary data
binCnt = 100
ax1 = plt.subplot(151)
ax1.hist(ax, bins=binCnt, edgecolor= 'black')
ax1.axvline(muAx, color= 'orange')
ax1.set_title("Ax")

ax2 = plt.subplot(152)
ax2.hist(ay, bins=binCnt, edgecolor= 'black')
ax2.axvline(muAy, color= 'orange')
ax2.set_title("Ay")

ax3 = plt.subplot(153)
ax3.hist(wz, bins=binCnt, edgecolor= 'black')
ax3.axvline(muWz, color= 'orange')
ax3.set_title("Wz")

ax4 = plt.subplot(154)
ax4.hist(theta_accel, bins=binCnt, edgecolor= 'black')
ax4.axvline(muThetaAccel, color= 'orange')
ax4.set_title("Theta Accel")

ax5 = plt.subplot(155)
ax5.hist(theta_gyro, bins=binCnt, edgecolor= 'black')
ax5.axvline(muThetaGyro, color= 'orange')
ax5.set_title("Theta Gyro")

plt.show()