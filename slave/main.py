import matplotlib.pyplot as plt
import numpy as np


def func(theta):
    return np.sqrt(2 / 3) * (
        4.27 * np.sin(theta) - 1.54 * np.sin(theta - 120) - 2.96 * np.sin(theta + 120)
    )


x = np.linspace(0, 360, 360)
y = []

for i in range(360):
    y.append(func(i))

plt.plot(x, y)
plt.show()
