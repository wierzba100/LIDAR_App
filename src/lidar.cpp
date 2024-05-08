#include "../hdr/lidar.hpp"

const struct uart_config uart_cfg = {
	.baudrate = 115200,
	.parity = UART_CFG_PARITY_NONE,
	.stop_bits = UART_CFG_STOP_BITS_1,
	.data_bits = UART_CFG_DATA_BITS_8,
	.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

Lidar::Lidar(const struct device *uart_dev)
{
    lidar = uart_dev;

    if (!device_is_ready(lidar))
	{
    	printk("Error\n");
	}

	if (uart_configure(lidar, &uart_cfg) == -ENOSYS)
    {
		printk("Error\n");
	}
}

bool Lidar::read_data(uint16_t* distance, uint16_t* strength)
{
    uint32_t checksum = 0;

    for (unsigned int i = 0; i < sizeof(rx_buf); i++)
    {
        uart_poll_in(lidar, &rx_buf[i]);
    }

    for (unsigned int i = 0; i < sizeof(rx_buf) - 1; i++)
    {
        checksum = checksum + rx_buf[i];
    }

    if ( (rx_buf[0] == 0x59) && (rx_buf[1] == 0x59) )//&& ((checksum % 256) == rx_buf[8]) )
    {
        *distance = rx_buf[2] + rx_buf[3] * 256;
        *strength = rx_buf[4] + rx_buf[5] * 256;

        return true;
    }else
    {
        return false;
    }


}