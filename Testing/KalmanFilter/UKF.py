import numpy as np
from filterpy.common import Q_discrete_white_noise
from filterpy.kalman import UnscentedKalmanFilter
from filterpy.kalman import MerweScaledSigmaPoints, unscented_transform
import matplotlib.pyplot as plt
from math import sqrt

def CalcGyroAngle(t, WzData, IC):
    theta = np.empty_like(t)
    
    lastAngle = IC
    lastT = t[0]
    for i in range(len(t) - 1):
        currentTime = t[i + 1]
        dt =  (currentTime - lastT)
    
        currentAngle = lastAngle + WzData[i + 1] * dt
        
        theta[i] = currentAngle
        
        lastT = currentTime
        lastAngle = currentAngle
    
    return theta

def CalcAccelAngle(AxData, AyData):
    theta = np.empty_like(AxData)
    
    for i in range(len(AxData)):
        theta_rad = np.arctan2(AxData[i], AyData[i])
        theta[i] = np.rad2deg(theta_rad)
    
    return theta

def CalcMahalanobis(kf, z, R, hx):
    sigmas_h = []
    for s in ukf.sigmas_f:
        sigmas_h.append(hx(s))

    sigmas_h = np.atleast_2d(sigmas_h)

    zp, S = unscented_transform(sigmas_h, ukf.Wm, ukf.Wc, R, ukf.z_mean, ukf.residual_z)
    SI = ukf.inv(S)
    
    y = ukf.residual_z(z, zp)   # residual
    
    mlbs = sqrt(float(np.dot(np.dot(y.T, SI), y)))
    return mlbs


zs = np.genfromtxt("Data/MotorFlipSample.log", delimiter=",", dtype=np.float32)
time = zs[:,0] / 1000
ax = zs[:,1]
ay = zs[:,2]
wz = zs[:,3]

g = 9.81

# Initial state estimate
theta_0 = 45
theta_dot_0 = 0
theta_dot_dot_0 = 0

# Initial state confidence
theta_var_0 = 5
theta_dot_var_0 = 10
theta_dot_dot_var_0 = 10

# Process noise
delta_theta_dot = 50**2

# Measurement noise
roll_var = 5 ** 2
ax_var = 5 ** 2
ay_var = 5 ** 2

# Sigma points
alpha = .1
beta = 1.
kappa = .1

def fx(x, dt):
    F = np.array([[1, dt, .5*dt**2],
                  [0,  1, dt],
                  [0, 0, 1]])
    return F @ x

def hx_1(x):
    zout = np.empty(1)
    zout[0] = x[1]
    return zout

def hx_2(x):
    zout = np.empty(2)
    x_rad = np.deg2rad(x[0])
    zout[0] = 1 * g * np.sin(x_rad)
    zout[1] = 1 * g * np.cos(x_rad)
    return zout

R_1 = np.array([roll_var])
R_2 = np.diag([ax_var, ay_var])

points = MerweScaledSigmaPoints(n=3, alpha= alpha, beta= beta, kappa= kappa)

ukf = UnscentedKalmanFilter(dim_x=3, fx= fx, dim_z=1, hx=hx_1, dt=.001, points= points)
ukf.x = np.array([theta_0, theta_dot_0, theta_dot_dot_0])
ukf.P = np.diag([theta_var_0, theta_dot_var_0, theta_dot_dot_var_0])

xs = []
lastTime = time[0]
count = 0
for i in range(len(time) - 1):
    currentTime = time[i+1]
    dt = currentTime - lastTime
    ukf.Q = Q_discrete_white_noise(3, dt = dt, var=delta_theta_dot)
    
    ukf.predict(dt=dt)
    
    ukf._dim_z = 1
    z1 = np.array([wz[i+1]])
    mal = CalcMahalanobis(ukf, z1, R_1, hx_1)
    
    if mal < 4.5:
        ukf.update(z1, hx=hx_1, R=R_1)
     
    ukf._dim_z = 2
    z2 = np.array([ax[i+1], ay[i+1]])
    mal = CalcMahalanobis(ukf, z2, R_2, hx_2)
    
    if mal < 4.5:
        ukf.update(z2, hx=hx_2, R=R_2)
    
    xs.append(ukf.x)
    
    print(i)
    print(mal)
    lastTime = currentTime
    
print(count)
xs = np.array(xs)

thetaAccel = CalcAccelAngle(ax,ay)
thetaGyro = CalcGyroAngle(time, wz, 45)
thetaGyro = thetaGyro[:-1]

ax1 = plt.subplot(211)
ax1.plot(time[1:],xs[:, 0])
ax1.plot(time[:-1], thetaGyro)
# ax1.plot(time, thetaAccel)
ax1.grid(True)

ax2 = plt.subplot(212)
ax2.plot(time[1:],xs[:, 1])
ax2.plot(time, wz)
ax2.grid(True)

plt.show()



