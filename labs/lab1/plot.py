# description: plot the data of timing CPU & minor CPU
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    timing_cpu_data = np.array([
        0.000183, 0.000170, 0.000159, 0.000149,
        0.000141, 0.000135, 0.000129, 0.000123,
        0.000118, 0.000114, 0.000110, 0.000107,
        0.000104, 0.000101, 0.000098, 0.000096,
        0.000093, 0.000091, 0.000089, 0.000088
    ])

    minor_cpu_data = np.array([
        0.000132, 0.000124, 0.000117, 0.000111,
        0.000106, 0.000102, 0.000097, 0.000093,
        0.000091, 0.000088, 0.000085, 0.000083,
        0.000081, 0.000080, 0.000077, 0.000076,
        0.000074, 0.000073, 0.000072, 0.000071
    ])

    x = np.arange(1000, 3000, 100)
    plt.plot(x, timing_cpu_data, 'r', label='timing CPU', marker='o')
    plt.plot(x, minor_cpu_data, 'b', label='minor CPU', marker='o')
    plt.xlabel('frequency (MHz)')
    plt.ylabel('sim_seconds (s)')
    plt.title('CPU performance')
    plt.legend()
    plt.show()