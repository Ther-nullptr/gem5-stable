# description: plot the data of timing CPU & minor CPU
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':

    minor_cpu_data = np.array([
        0.000132,
        0.000106,
        0.000091,
        0.000081,
        0.000074,
    ])

    minor_cpu_data_int_optimize = np.array([
        0.000128,
        0.000103,
        0.000088,
        0.000079,
        0.000073,
    ])

    minor_cpu_data_float_optimize = np.array([
        0.000132,
        0.000106,
        0.000091,
        0.000081,
        0.000074,
    ])

    x = np.arange(1000, 3000, 400)
    plt.plot(x, minor_cpu_data, 'r', label='minor CPU', marker='o')
    plt.plot(x, minor_cpu_data_int_optimize, 'b', label='minor CPU int optimize', marker='o')
    plt.plot(x, minor_cpu_data_float_optimize, 'g', label='minor CPU float optimize', marker='o')
    plt.xlabel('frequency (MHz)')
    plt.ylabel('sim_seconds (s)')
    plt.title('CPU performance')
    plt.legend()
    plt.show()