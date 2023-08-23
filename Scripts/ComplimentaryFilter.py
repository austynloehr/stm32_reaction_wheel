import matplotlib.pyplot as plt
from LogPlotter import parse
from scipy import integrate, signal
import pandas as pd
import numpy as np
import readline
import os.path
import sys

calcErrorCoeff_bool = 0
alpha = .90

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

    # Calculate orientation based on accel and gyro measurements
    thetaAccel = CalcAccelAngles(df)
    thetaGyro = CalcGyroAngles(df)

    # Calculate complimentary filter
    thetaEst = ComplimentaryFilter(thetaAccel, thetaGyro, alpha)

    # Plot data
    fig1, axs = plt.subplots(2, 1, sharex=True, layout='constrained')

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

    fig1.legend(["Angle Accel", "Angle Gyro", "Accel Est"], loc='outside right upper')
    fig1.suptitle('Acceleration / Gyro Orientation Data', fontsize=16)
    fig1.supxlabel('Samples',fontsize=16)

    plt.show()

def CalcAccelAngles(df: pd.DataFrame):
    Ax = df["AX"].to_numpy()
    Ay = df["AY"].to_numpy()
    Az = df["AZ"].to_numpy()

    roll = np.rad2deg(np.arctan2(Ay, Az))
    pitch = np.rad2deg(np.arctan2(-1 * Ax, np.sqrt(np.square(Ay) + np.square(Az))))
    yaw = np.zeros(Ax.size)  # Create array of zeros

    theta = np.array([roll, pitch, yaw])

    return theta

def CalcGyroAngles(df: pd.DataFrame):
    dt = .030

    Wx = df["WX"].to_numpy()
    Wy = df["WY"].to_numpy()
    Wz = df["WZ"].to_numpy()

    roll = integrate.cumtrapz(Wx, dx= dt, initial= 0)
    pitch = integrate.cumtrapz(Wy, dx= dt, initial= 0)
    yaw = integrate.cumtrapz(Wz, dx= dt, initial= 0)

    theta = np.array([roll, pitch, yaw])

    return theta

def ComplimentaryFilter(thetaAccel: np.ndarray, thetaGyro: np.ndarray, alpha: float):
    filt_order = 1
    dt = .03

    alphaFiltAccel = .1
    alphaFiltGyro = .1

    b1, a1 = signal.butter(filt_order, 2, btype= 'lowpass',fs= 1/dt)
    b2, a2 = signal.butter(filt_order, .001, btype= 'highpass',fs= 1/dt)

    thetaAccel_filt = signal.filtfilt(b1, a1, thetaAccel)
    thetaGyro_filt = signal.filtfilt(b2, a2, thetaGyro)

    rollAccel_filt = thetaAccel[0] * alphaFiltAccel + (1 - alphaFiltAccel) * thetaAccel_filt[0]
    pitchAccel_filt = thetaAccel[1] * alphaFiltAccel + (1 - alphaFiltAccel) * thetaAccel_filt[1]

    rollGyro_filt = thetaGyro[0] * alphaFiltGyro + (1 - alphaFiltGyro) * thetaGyro_filt[0]
    pitchGyro_filt = thetaGyro[1] * alphaFiltGyro + (1 - alphaFiltGyro) * thetaGyro_filt[1]

    roll = alpha * rollGyro_filt + (1 - alpha) * rollAccel_filt
    pitch = alpha * pitchGyro_filt + (1 - alpha) * pitchAccel_filt

    theta = np.array([roll, pitch])

    # Plots for debugging
    fig1, axs1 = plt.subplots(2, 1, sharex=True, layout='constrained')

    axs1[0].plot(np.arange(len(thetaAccel[0])), thetaAccel[0])
    axs1[0].plot(np.arange(len(thetaGyro[0])), thetaGyro[0], color= "red")
    axs1[0].plot(np.arange(len(thetaAccel_filt[0])), thetaAccel_filt[0], color= "orange")
    axs1[0].plot(np.arange(len(thetaGyro_filt[0])), thetaGyro_filt[0], color= "purple")
    
    axs1[0].set_ylabel('Roll (deg)')
    axs1[0].grid(True)

    axs1[1].plot(np.arange(len(thetaAccel[1])), thetaAccel[1])
    axs1[1].plot(np.arange(len(thetaGyro[1])), thetaGyro[1], color= "red")
    axs1[1].plot(np.arange(len(thetaAccel_filt[1])), thetaAccel_filt[1], color= "orange")
    axs1[1].plot(np.arange(len(thetaGyro_filt[1])), thetaGyro_filt[1], color= "purple")
    axs1[1].set_ylabel('Pitch (deg)')
    axs1[1].grid(True)

    fig1.suptitle('Butterworth Filter Debug', fontsize=16)
    fig1.supxlabel('Samples',fontsize=16)

    fig1.legend(["Angle Accel RAW", "Angle Gyro RAW", "Accel Filt", "Gyro Filt"], loc='outside right upper')
    plt.show()

    fig2, axs2 = plt.subplots(2, 1, sharex=True, layout='constrained')

    axs2[0].plot(np.arange(len(thetaAccel[0])), thetaAccel[0])
    axs2[0].plot(np.arange(len(thetaGyro[0])), thetaGyro[0], color= "red")
    axs2[0].plot(np.arange(len(rollAccel_filt)), rollAccel_filt, color= "orange")
    axs2[0].plot(np.arange(len(rollGyro_filt)), rollGyro_filt, color= "purple")
    
    axs2[0].set_ylabel('Roll (deg)')
    axs2[0].grid(True)

    axs2[1].plot(np.arange(len(thetaAccel[1])), thetaAccel[1])
    axs2[1].plot(np.arange(len(thetaGyro[1])), thetaGyro[1], color= "red")
    axs2[1].plot(np.arange(len(pitchAccel_filt)), pitchAccel_filt, color= "orange")
    axs2[1].plot(np.arange(len(pitchGyro_filt)), pitchGyro_filt, color= "purple")
    axs2[1].set_ylabel('Pitch (deg)')
    axs2[1].grid(True)

    fig2.suptitle('Combined RAW + Filtered Debug', fontsize=16)
    fig2.supxlabel('Samples',fontsize=16)

    fig2.legend(["Angle Accel RAW", "Angle Gyro RAW", "Combined Accel", "Combined Gyro"], loc='outside right upper')
    plt.show()

    return theta

if __name__=='__main__':
    main()