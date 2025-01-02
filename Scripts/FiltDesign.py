from scipy import signal
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Time parameters
    dt = .010
    fs = 1/dt

    # FIR filter parameters
    numtaps1 = 99
    numtaps2 = 20

    # Generate filter coefficients
    #FIR filters
    b1 = signal.firwin(numtaps1, .5, fs=fs, window=('kaiser', 0), pass_zero='highpass')
    b2 = signal.firwin(numtaps2, 20, fs=fs, window='hamming', pass_zero='lowpass')
   
    #IIR filters
    sos1 = signal.butter(2, 20, btype='lowpass', output='sos', fs=fs)
    
    # Analyze response
    w1, h1 = signal.freqz(b1, fs=fs)
    w2, h2 = signal.freqz(b2, fs=fs)
    w3, h3 = signal.sosfreqz(sos1, fs=fs)


    # Plot response
    # plot_freq_response(w1, h1, Wc=.5, fs=fs)
    # plot_freq_response(w2, h2, Wc=20, fs=fs)
    plot_freq_response(w3, h3, Wc=20, fs=fs)


    # Py2CCoeffArray('Resources/HpFirFIltCoef.txt', b=b1)
    # Py2CCoeffArray('Resources/LpFirFIltCoef.txt', b=b2)
    Py2CCoeffArray('Scripts/LpIirFIltCoef.txt', sos=sos1)

    print(sos1)


def plot_freq_response(w: np.ndarray, h: np.ndarray, Wc: float, fs: float):
    plt.semilogx(w, 20 * np.log10(abs(h)))

    plt.title('Filter Frequency Response')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
    plt.grid(which='both', axis='both')

    plt.axvline(Wc, color='green')  # cutoff frequency

    plt.show()

def Py2CCoeffArray(path:str, **kwargs):
    sos = kwargs.get('sos', None)
    b = kwargs.get('b', None)

    f = open(path, 'w')

    if sos[0] is not None:
        count = 0
        for i, coeff in enumerate(sos[0]):
            if i == 0:
                f.write('sos = {' + str(coeff) + ', ')
            elif i == (len(sos[0]) - 1):
                f.write(str(coeff) + '}')
            else:
                f.write(str(coeff) + ', ')
            
            count += 1

            if count == 6:
                f.write('\n')
                count = 0

    if b is not None:
        fbCount = 0
        for i, coeff in enumerate(b):
            if i == 0:
                f.write('b = {' + str(coeff) + ', ')
            elif i == (len(b) - 1):
                f.write(str(coeff) + '}')
            else:
                f.write(str(coeff) + ', ')
            
            fbCount += 1

            if fbCount == 6:
                f.write('\n')
                fbCount = 0
    



if __name__=='__main__':
    main()