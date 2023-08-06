import matplotlib.pyplot as plt
from LogPlotter import parse
from scipy import integrate, signal
import pandas as pd
import numpy as np
import readline
import os.path
import sys


def main():
    userInputReq = len(sys.argv)  < 2

    if userInputReq: 
        readline.set_completer_delims(' \t\n=')  # CL input delimiters
        readline.parse_and_bind("tab: complete") # Call tab complete function
        path = input("Enter log path: ")  # Get user input with file complete
    else:
        path = sys.argv[1]

    # Raise error if file does not exist
    if not os.path.isfile(path):
        raise FileNotFoundError

    # Parse file
    df = parse(path)

    thetaAccel = CalcAccelAngles(df)
    thetaGyro = CalcGyroAngles(df)

    thetaEst = ComplimentaryFilter(thetaAccel, thetaGyro, .30)

    fig1, axs = plt.subplots(2, 1, sharex=True)

    axs[0].plot(np.arange(len(thetaAccel[0])), thetaAccel[0])
    axs[0].plot(np.arange(len(thetaGyro[0])), thetaGyro[0], color= "red")
    axs[0].plot(np.arange(len(thetaEst[0])), thetaEst[0], color= "orange")
    axs[0].set_ylabel('Roll (deg)')
    axs[0].grid(True)

    axs[1].plot(np.arange(len(thetaAccel[1])), thetaAccel[1])
    axs[1].plot(np.arange(len(thetaGyro[1])), thetaGyro[1], color= "red")
    axs[1].plot(np.arange(len(thetaEst[1])), thetaEst[1], color= "orange")
    axs[1].set_ylabel('Pitch (deg)')
    axs[1].grid(True)

    fig1.suptitle('Acceleration / Gyro Orientation Data', fontsize=16)
    fig1.supxlabel('Samples',fontsize=16)

    plt.show()

    print(type(thetaAccel))


def CalcAccelAngles(df: pd.DataFrame):
    Ax = df["AX"].to_numpy()
    Ay = df["AY"].to_numpy()
    Az = df["AZ"].to_numpy()

    roll = np.rad2deg(np.arctan2(Ay, Az))
    pitch = np.rad2deg(np.arctan2(-1 * Ax, np.sqrt(np.square(Ay) + np.square(Az))))
    yaw = np.zeros(Ax.size)  # Create array of zeros

    theta = np.array([roll, pitch, yaw])

    return theta

def CalcGyroAngles(df: pd.DataFrame, thetaAccel: pd.DataFrame):
    dt = .030

    Wx = df["WX"].to_numpy()
    Wy = df["WY"].to_numpy()
    Wz = df["WZ"].to_numpy()

    rollAccel = thetaAccel[0]
    pitchAccel = thetaAccel[1]

    roll = [0] * len(Wx)
    pitch = [0] * len(Wy)
    yaw = [0] * len(Wz)

    # roll = integrate.cumtrapz(Wx, dx= dt, initial= 0)
    # pitch = integrate.cumtrapz(Wy, dx= dt, initial= 0)
    # yaw = integrate.cumtrapz(Wz, dx= dt, initial= 0)

    constRollCnt = 0

    for i, data in enumerate(Wx):
        if i == 0:
            roll[i] = 0
        else:
            roll[i] = roll[i -1] + data * dt

    for i, data in enumerate(Wy):
        if i == 0:
            pitch[i] = 0
        else:
            if pitchAccel[i] in [pitchAccel[i-1] * .95, pitchAccel[i-1] * 1.05]:
                roll[i] = pitchAccel[i]
            else:
                roll[i] = roll[i -1] + data * dt

    for i, data in enumerate(Wz):
        if i == 0:
            yaw[i] = 0
        else:
            yaw[i] = yaw[i -1] + data * dt

    theta = np.array([roll, pitch, yaw])

    return theta

def ComplimentaryFilter(thetaAccel: np.ndarray, thetaGyro: np.ndarray, alpha: float):
    filt_order = 1
    dt = .03

    alphaFiltAccel = .2
    alphaFiltGyro = .5

    b1, a1 = signal.butter(filt_order, 1, btype= 'lowpass',fs= 1/dt)
    b2, a2 = signal.butter(filt_order, .01, btype= 'highpass',fs= 1/dt)

    thetaAccel_filt = signal.filtfilt(b1, a1, thetaAccel)
    thetaGyro_filt = signal.filtfilt(b2, a2, thetaGyro)

    rollAccel_filt = thetaAccel[0] * alphaFiltAccel + (1 - alphaFiltAccel) * thetaAccel_filt[0]
    pitchAccel_filt = thetaAccel[1] * alphaFiltAccel + (1 - alphaFiltAccel) * thetaAccel_filt[1]

    rollGyro_filt = thetaGyro[0] * alphaFiltGyro + (1 - alphaFiltGyro) * thetaGyro_filt[0]
    pitchGyro_filt = thetaGyro[1] * alphaFiltGyro + (1 - alphaFiltGyro) * thetaGyro_filt[1]

    roll = alpha * rollGyro_filt + (1 - alpha) * rollAccel_filt
    pitch = alpha * pitchGyro_filt + (1 - alpha) * pitchAccel_filt

    theta = np.array([roll, pitch])

    return theta

if __name__=='__main__':
    main()