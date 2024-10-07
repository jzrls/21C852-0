# import matplotlib

# matplotlib.use("TkAgg")

import matplotlib.pyplot as plt
import numpy as np


k = 1
b = 0


def get_iq(theta, ia, ib, ic):
    return np.sqrt(2 / 3) * (
        (k * ia + b) * np.sin(theta)
        + (k * ib + b) * np.sin(theta - 120)
        + (k * ic + b) * np.sin(theta + 120)
    )


def get_id(theta, ia, ib, ic):
    return np.sqrt(2 / 3) * (
        (k * ia + b) * np.cos(theta)
        + (k * ib + b) * np.cos(theta - 120)
        + (k * ic + b) * np.cos(theta + 120)
    )


x = np.linspace(0, 360, 360)
y = []
for i in range(360):
    i_d = get_id(i, -4.35, 4.53, -0.31)
    i_q = get_iq(i, -4.35, 4.53, -0.31)
    i_motor = np.sqrt(i_d**2 + i_q**2)
    y.append(i_motor)
    print("{}, {}, {}".format(i_d, i_q, i_motor))


plt.plot(x, y)
plt.show()
