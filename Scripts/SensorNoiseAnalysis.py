import numpy as np
from matplotlib import pyplot as plt
from ReadBinaryLog import convert_log

logPath = 'Logs/40k_noiseLog.log'

# Struct format
lengths = [4,4,4,4,4,1,1,1]
format = 'IfffiBBB'
labels = ['t','Ax','Ay','Wz','MotorSpeedReq','StateReq','CurrentState','MotorEnable']

df = convert_log(format,lengths,labels,logPath)
print(df)
# mu, std
muAx, stdAx = [np.mean(df['Ax']), np.std(df['Ax'])]
muAy, stdAy = [np.mean(df['Ay']), np.std(df['Ay'])]
muWz, stdWz = [np.mean(df['Wz']), np.std(df['Wz'])]
print(stdAx)
print(stdAy)
print(stdWz)

# Create histograms for stationary data
binCnt = 100
ax1 = plt.subplot(131)
ax1.hist(df['Ax'], bins=binCnt, edgecolor= 'black')
ax1.axvline(muAx, color= 'orange')
ax1.set_title("Ax")

ax2 = plt.subplot(132)
ax2.hist(df['Ay'], bins=binCnt, edgecolor= 'black')
ax2.axvline(muAy, color= 'orange')
ax2.set_title("Ay")

ax3 = plt.subplot(133)
ax3.hist(df['Wz'], bins=binCnt, edgecolor= 'black')
ax3.axvline(muWz, color= 'orange')
ax3.set_title("Wz")

plt.show()