import asyncio
import numpy as np
from bleak import BleakClient, BleakScanner
from bleak.backends.characteristic import BleakGATTCharacteristic
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import threading


class HorizontalAndVerticalDegsPlusFlag:
    def __init__(self, horizontal_deg, vertical_deg, flag_to_stop):
        self.horizontal_deg = horizontal_deg
        self.vertical_deg = vertical_deg
        self.flag_to_stop = flag_to_stop

degs_plus_flag = HorizontalAndVerticalDegsPlusFlag(0, -45, 1)

x_data = []
y_data = []
z_data = []


def update_graph_data(azimuth_angle, elevation_angle, distance):
    azimuth_angle_rad = np.deg2rad(azimuth_angle)
    elevation_angle_rad = np.deg2rad(elevation_angle)

    x = distance * np.cos(elevation_angle_rad) * np.cos(azimuth_angle_rad)
    y = distance * np.cos(elevation_angle_rad) * np.sin(azimuth_angle_rad)
    z = distance * np.sin(elevation_angle_rad)
    
    x_data.append(x)
    y_data.append(y)
    z_data.append(z)


async def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    decimal_values = [int(byte) for byte in data]
    distance = (decimal_values[0] << 8) + decimal_values[1]
    strength = (decimal_values[2] << 8) + decimal_values[3]
    
    if distance > 1200:
        distance = 1200

    update_graph_data(degs_plus_flag.horizontal_deg, degs_plus_flag.vertical_deg, distance)
    degs_plus_flag.horizontal_deg += 1
    if degs_plus_flag.horizontal_deg == 180:
        degs_plus_flag.horizontal_deg = 0
        degs_plus_flag.vertical_deg += 1
        if degs_plus_flag.vertical_deg == 44:
            degs_plus_flag.flag_to_stop = 0


async def client_loop(lidar_scanner: BleakClient):
    async with BleakClient(lidar_scanner) as client:
        print("Connected")
        await client.write_gatt_char("932ee1e0-547a-4ef8-93f7-843ffa16afab", b'\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11', False)
        await client.start_notify("932ee1df-547a-4ef8-93f7-843ffa16afab", notification_handler)
        while degs_plus_flag.flag_to_stop:
            await asyncio.sleep(1.0)
        await client.stop_notify("932ee1df-547a-4ef8-93f7-843ffa16afab")
        await client.write_gatt_char("932ee1e0-547a-4ef8-93f7-843ffa16afab", b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF', False)


def run_client_loop(lidar_scanner):
    asyncio.run(client_loop(lidar_scanner))


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    lidar_scanner = loop.run_until_complete(BleakScanner.find_device_by_name("LIDAR_SCANNER", timeout=60.0))

    ax = plt.axes(projection='3d')

    ax.set_xlim([-1200, 1200])
    ax.set_ylim([-1200, 1200])
    ax.set_zlim([-1200, 1200])

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    if lidar_scanner is None:
        print("Device not found!")
    else:
        client_thread = threading.Thread(target=run_client_loop, args=(lidar_scanner,))
        client_thread.start()
        
        plt.ion()
        while degs_plus_flag.flag_to_stop:
            ax.scatter(x_data, y_data, z_data, c='r', marker='o', s=1)
            plt.draw()
            plt.pause(0.5)
        
        plt.savefig("scan.png")
        plt.ioff()
        plt.show()
