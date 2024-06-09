import serial
import numpy as np
from matplotlib import pyplot as plt


def update_graph(x, y, z):
    x_data.append(x)
    y_data.append(y)
    z_data.append(z)
    ax.scatter(x_data, y_data, z_data, c='r', marker='o', s=20)
    plt.draw()

ax = plt.axes(projection='3d')

ax.set_xlim([0, 180])
ax.set_ylim([0, 65535])
ax.set_zlim([0, 90])

x_data = []
y_data = []
z_data = []

plt.ion()
plt.show()

ser = serial.Serial("COM5", 115200, timeout=1)

for z in range(90):
    for x in range(180):
        raw_data = ser.read(7)
        data = int(raw_data.strip())
        update_graph(x, data, z)
        plt.pause(1)


ser.close()
