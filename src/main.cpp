#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <memory>
#include "../hdr/servo.hpp"
#include "../hdr/lidar.hpp"

int main(void)
{
	pwm_dt_spec pwm_servo_x = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_x));
	std::unique_ptr<Servo> servo_x = std::make_unique<Servo>(pwm_servo_x);

	pwm_dt_spec pwm_servo_y = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_y));
	std::unique_ptr<Servo> servo_y = std::make_unique<Servo>(pwm_servo_y);

	const struct device *uart_device = DEVICE_DT_GET(DT_NODELABEL(uart1));
	std::unique_ptr<Lidar> lidar = std::make_unique<Lidar>(uart_device);

	k_msleep(1000);

	servo_x->GoTo(0);
	servo_y->GoTo(90);

	uint16_t distance = 0, strength = 0;

	while(1)
	{
		if(lidar->read_data(&distance, &strength))
		{
			printk("Distance: %d cm\n", distance);
			printk("Strength: %d \n", strength);
			printk("\n");
		}
		k_msleep(50);
	}

	return 0;
}