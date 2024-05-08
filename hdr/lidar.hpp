#ifndef LIDAR_H
#define LIDAR_H

#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

#define RX_BUFFER_SIZE 9

class Lidar {
    private:
        unsigned char rx_buf[RX_BUFFER_SIZE] = {0};
        const struct device *lidar;

    public:
        Lidar(const struct device *uart_dev);
        bool read_data(uint16_t* distance, uint16_t* strength);
};

#endif