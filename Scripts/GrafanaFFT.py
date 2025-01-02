from numpy.fft import fft, ifft
import numpy as np
import matplotlib.pyplot as plt

# Using genfromtxt to read the CSV file
data = np.genfromtxt('Logs/pterm_filt_data.csv', delimiter=',', skip_header=1)
data[:,0] -= data[0,0]

sr = 100
ts = 1/sr
t = data[:,0]

x = data[:,1]

X = fft(x)
N = len(X)
n = np.arange(N)
T = N/sr
freq = n/T

plt.figure(figsize = (12, 6))
plt.subplot(121)

plt.stem(freq, np.abs(X), 'b', \
         markerfmt=" ", basefmt="-b")
plt.xlabel('Freq (Hz)')
plt.ylabel('FFT Amplitude |X(freq)|')
plt.xlim(0, 50)

plt.subplot(122)
plt.plot(t, ifft(X), 'r')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.tight_layout()
plt.show()