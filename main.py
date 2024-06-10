import time
import asyncio
from bleak import BleakClient, BleakScanner
from bleak.backends.characteristic import BleakGATTCharacteristic
from matplotlib import pyplot as plt

class X_and_z_vars:
    def __init__(self, x, z, flag_to_stop):
        self.x = x
        self.z = z
        self.flag_to_stop = flag_to_stop

x_and_z_vars = X_and_z_vars(0, 0, 1)
def update_graph(x, y, z):
    x_data.append(x)
    y_data.append(y)
    z_data.append(z)
    ax.scatter(x_data, y_data, z_data, c='r', marker='o', s=20)
    plt.draw()
    plt.pause(0.5)
async def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    decimal_values = [int(byte) for byte in data]
    distance = (decimal_values[0] << 8) + decimal_values[1]
    strength = (decimal_values[2] << 8) + decimal_values[3]
    print(strength)
    if(distance > 1200):
        distance = 1200

    update_graph(x_and_z_vars.x, distance, x_and_z_vars.z)
    x_and_z_vars.x = x_and_z_vars.x + 1
    if(x_and_z_vars.x == 180):
        x_and_z_vars.x = 0
        x_and_z_vars.z = x_and_z_vars.z + 1
        if(x_and_z_vars.z == 135):
            x_and_z_vars.flag_to_stop = 0

async def client_loop(lidar_scanner: BleakClient):
    async with BleakClient(lidar_scanner) as client:
        print("Connected")
        await client.write_gatt_char("932ee1e0-547a-4ef8-93f7-843ffa16afab", b'\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11', False)
        await client.start_notify("932ee1df-547a-4ef8-93f7-843ffa16afab", notification_handler)
        while(x_and_z_vars.flag_to_stop):
            await asyncio.sleep(1.0)
        await client.stop_notify("932ee1df-547a-4ef8-93f7-843ffa16afab")
        await client.write_gatt_char("932ee1e0-547a-4ef8-93f7-843ffa16afab", b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF', False)


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    lidar_scanner = loop.run_until_complete(BleakScanner.find_device_by_name("LIDAR_SCANNER", timeout=60.0))

    ax = plt.axes(projection='3d')

    ax.set_xlim([0, 180])
    ax.set_ylim([0, 1200])
    ax.set_zlim([0, 90])

    x_data = []
    y_data = []
    z_data = []

    if (lidar_scanner == None):
        print("Device not found!")
    else:
        loop.run_until_complete(client_loop(lidar_scanner))

