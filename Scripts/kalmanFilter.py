import numpy as np
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import KalmanFilter, UnscentedKalmanFilter
from filterpy.common import Saver
from ReadBinaryLog import convert_log
import kalmanFunctions as fc
import matplotlib.pylab as plt
from scipy.signal import butter, filtfilt
from numpy.linalg import inv

# Function definitions
def butter_lowpass_filter(data, cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

def FirstOrderKF(gateBool):
    kf = KalmanFilter(dim_x=2, dim_z=1)
    kf.x = np.array([[theta_0,thetaDot_0]]).T
    kf.P *= np.array([[pTheta,0],[0,pThetaDot]])

    kf.F = np.array([[1,dt],
                 [0,1]])
    kf.Q = Q_discrete_white_noise(dim=2,dt=dt,var=stdQ**2)
    
    xs = []
    for i in range(len(t)):
        kf.predict()
        
        kf.H = np.array([[0,1]])
        kf.R = np.array([[stdWz ** 2]])
        kf.update(wz[i])
        
        std_theta = np.sqrt(kf.P[0,0])
        gateLimit = 7
        kf.R = np.array([[stdTheta ** 2]])
        kf.H = np.array([[1,0]])
        y = kf.residual_of(thetaAccelFilt[i])[0]
        
        if gateBool:
            if abs(y[0]) > gateLimit * std_theta:
                pass
                # print('Discarding')
                # print(thetaAccelFilt[i])
                # print(abs(y[0]))
                # print(gateLimit * std_theta)
            else: 
                kf.update(thetaAccelFilt[i])
        else:
            kf.update(thetaAccelFilt[i])
        
        xs.append(kf.x)
    xs = np.array(xs)
    return xs

def SecondOrderKF(gateBool, gateLimit):
    kf = KalmanFilter(dim_x=3, dim_z=1)
    kf.x = np.array([[theta_0,thetaDot_0,thetaDotDot_0]]).T
    kf.P [0,0] = pTheta
    kf.P [1,1] = pThetaDot
    kf.P [2,2] = pThetaDotDot

    kf.F = np.array([[1,dt,.5*dt**2],
                 [0,1,dt],
                 [0,0,1]])
    kf.Q = Q_discrete_white_noise(dim=3,dt=dt,var=stdQ**2)
    
    xs = []
    ys = []
    ps = []
    epss = []
    for i in range(len(t)):
        kf.predict()
        
        kf.H = np.array([[1,0,0]])
        kf.R = np.array([[stdTheta ** 2]])
        y1 = kf.residual_of(thetaAccelFilt[i])[0][0]
        std_theta = np.sqrt(kf.P[0,0])
        if gateBool:
            if abs(y1) > gateLimit * std_theta:
                pass
            else: 
                kf.update(thetaAccelFilt[i])
                eps1 = kf.y.T @ inv(kf.S) @ kf.y
        else:
            kf.update(thetaAccelFilt[i])
            eps1 = kf.y.T @ inv(kf.S) @ kf.y

        kf.H = np.array([[0,1,0]])
        kf.R = np.array([[stdWz ** 2]])
        y2 = kf.residual_of(wz[i])[0][0]
        kf.update(wz[i])
        eps2 = kf.y.T @ inv(kf.S) @ kf.y
        
        ys.append([y1, y2])
        xs.append(kf.x)
        ps.append(kf.P)
        epss.append([eps1[0], eps2[0]])
        
    xs = np.array(xs)
    ys = np.array(ys)
    ps = np.array(ps)
    epss = np.array(epss)
    return xs, ps, ys, epss

logPath = 'Logs/40k_flip.log'

# Struct format
lengths = [4,4,4,4,4,1,1,1]
format = 'IfffiBBB'
labels = ['t','Ax','Ay','Wz','MotorSpeedReq','StateReq','CurrentState','MotorEnable']

df = convert_log(format,lengths,labels,logPath)

# Assign variables for data
ax = df['Ax']
ay = df['Ay']
wz = df['Wz']
t = df['t'] / 1000

thetaGyro = fc.CalcGyroAngle(t*1000,wz,45)
thetaAccel = fc.CalcAccelAngle(ax,ay)

# Filter accel data and calculate angle
axFilt = butter_lowpass_filter(ax, 20, 500, 1)
ayFilt = butter_lowpass_filter(ay, 20, 500, 1)
thetaAccelFilt = fc.CalcAccelAngle(axFilt, ayFilt)

# Kalaman filtering
dt = .002

# Filter 1 inputs
theta_0 = 45
thetaDot_0 = 0
thetaDotDot_0 = 0

pTheta = 2
pThetaDot = 10
pThetaDotDot = 10

stdQ = 10
stdWz = 8
stdTheta = 8

x1,p1,y1,eps1 = SecondOrderKF(0, 50)
# gate1 = p1[:,0,0] * 50

axFilt = butter_lowpass_filter(ax, 10, 500, 1)
ayFilt = butter_lowpass_filter(ay, 10, 500, 1)
thetaAccelFilt = fc.CalcAccelAngle(axFilt, ayFilt)

# Filter 2 inputs
stdQ = 10
stdWz = 8
stdTheta = 8
x2,p2,y2,eps2 = SecondOrderKF(1, 75)
gate2 = p2[:,0,0] * 75

# Plotting
ax1 = plt.subplot(411)
ax1.plot(t,thetaAccelFilt,label='Accel Angle')
ax1.plot(t[:-10],thetaGyro[:-10],label='Gyro Angle')
ax1.plot(t,x1[:,0],label='Filter 1')
ax1.plot(t,x2[:,0],label='Filter 2')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Angle (deg)')
ax1.set_title('Angle Output')
ax1.legend()
ax1.grid()

ax2 = plt.subplot(412,sharex=ax1)
ax2.plot(t,wz,label='Wz')
ax2.plot(t,x1[:,1],label='Filter 1')
ax2.plot(t,x2[:,1],label='Filter 2')
ax2.set_xlabel('Time (s)')
ax2.set_ylabel('Angular Rate (deg/s)')
ax2.set_title('Angular Rate Output')
ax2.legend()
ax2.grid()

ax3 = plt.subplot(413,sharex=ax1)
ax3.plot(t,y1[:,0],label='Filter 1')
ax3.plot(t,y2[:,0],label='Filter 2')
# ax3.plot(t,gate1,label='Filter 1 Gate')
# ax3.plot(t,gate1 * -1,label='Filter 1 Gate')
ax3.plot(t,gate2,label='Filter 2 Gate')
ax3.plot(t,gate2 * -1,label='Filter 2 Gate')
ax3.set_xlabel('Time (s)')
ax3.set_ylabel('Angle (deg)')
ax3.set_title('Accel Angle Residual')
ax3.legend()
ax3.grid()

ax4 = plt.subplot(414,sharex=ax1)
ax4.plot(t,eps1[:,0],label='Filter 1')
ax4.plot(t,eps2[:,0],label='Filter 2')
ax4.set_xlabel('Time (s)')
ax4.set_ylabel('Angle (deg)')
ax4.set_title('Accel Epsilon')
ax4.legend()
ax4.grid()

# ax4 = plt.subplot(414,sharex=ax1)
# ax4.plot(t,y1[:,1],label='Filter 1')
# ax4.plot(t,y2[:,1],label='Filter 2')
# ax4.set_xlabel('Time (s)')
# ax4.set_ylabel('Angular Rate (deg/s)')
# ax4.set_title('Gyro Residual')
# ax4.legend()
# ax4.grid()

plt.show()