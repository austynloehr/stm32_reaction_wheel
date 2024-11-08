import numpy as np
import matplotlib.pylab as plt
from filterpy.kalman import KalmanFilter

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