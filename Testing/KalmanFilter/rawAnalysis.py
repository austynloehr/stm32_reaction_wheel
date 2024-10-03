import numpy as np
import matplotlib.pylab as plt
from filterpy.stats import gaussian

def CalcAccelAngle(AxData, AyData):
    theta = np.empty_like(AxData)
    
    for i in range(len(AxData)):
        theta_rad = np.arctan2(AxData[i], AyData[i])
        theta[i] = np.rad2deg(theta_rad)
    
    return theta

def CalcGyroAngle(time, WzData, IC):
    theta = np.empty_like(time)
    
    lastAngle = IC
    lastTime = time[0]
    for i in range(len(time) - 1):
        currentTime = time[i + 1]
        dt =  (currentTime - lastTime) / 1000
    
        currentAngle = lastAngle + WzData[i + 1] * dt
        
        theta[i] = currentAngle
        
        lastTime = currentTime
        lastAngle = currentAngle
    
    return theta

def CalcGyroAlpha(time, WzData):
    alpha = np.empty_like(time)
    
    for i in range(len(time) - 1):
        dt = (time[i+1] - time[i]) / 1000
        dtheta = WzData[i + 1] - WzData[i]
        
        alpha[i] = dtheta / dt
    
    return alpha
        
    

data_motor = np.genfromtxt("Data/MotorSweepNoise.log", delimiter=",", dtype=np.float32)
data_flip = np.genfromtxt("Data/SampleFlip2.log", delimiter=",", dtype=np.float32)

# Assign data
AxData_1 = data_motor[:,1]
AyData_1 = data_motor[:,2]
WzData_1 = data_motor[:,3]

time_2 = data_flip[:,0]
AxData_2 = data_flip[:,1]
AyData_2 = data_flip[:,2]
WzData_2 = data_flip[:,3]

# mu, std
muAx, stdAx = [np.mean(AxData_1), np.std(AxData_1)]
muAy, stdAy = [np.mean(AyData_1), np.std(AyData_1)]
muWz, stdWz = [np.mean(WzData_1), np.std(WzData_1)]
print(stdAx)
print(stdAy)
print(stdWz)

# Create histograms for stationary data
binCnt = 100
ax1 = plt.subplot(131)
ax1.hist(AxData_1, bins=binCnt, edgecolor= 'black')
ax1.axvline(muAx, color= 'orange')
ax1.set_title("Ax")

ax2 = plt.subplot(132)
ax2.hist(AyData_1, bins=binCnt, edgecolor= 'black')
ax2.axvline(muAy, color= 'orange')
ax2.set_title("Ay")

ax3 = plt.subplot(133)
ax3.hist(WzData_1, bins=binCnt, edgecolor= 'black')
ax3.axvline(muWz, color= 'orange')
ax3.set_title("Wz")

plt.show()

thetaAccel = CalcAccelAngle(AxData_2, AyData_2)
thetaGyro = CalcGyroAngle(time_2, WzData_2, 45)
alphaGyro = CalcGyroAlpha(time_2, WzData_2)

#ax4 = plt.plot(time_2, thetaAccel)
#ax4 = plt.plot(time_2, thetaGyro)
# ax4 = plt.plot(time_2, WzData_2)
ax4 = plt.subplot(311)
ax4.plot(time_2/1000, thetaGyro)
ax4.grid(True)
ax5 = plt.subplot(312, sharex= ax4)
ax5.plot(time_2/1000, WzData_2)
ax5.grid(True)
ax6 = plt.subplot(313, sharex= ax4)
ax6.plot(time_2/1000, alphaGyro)
ax6.grid(True)
plt.show()

