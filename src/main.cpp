#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include "../hdr/servo.hpp" 

int main(void)
{
	pwm_dt_spec pwm_servo_x = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_x));
	Servo Servo_x(pwm_servo_x);

	pwm_dt_spec pwm_servo_y = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_y));
	Servo Servo_y(pwm_servo_y);

	Servo_x.Callib();
	Servo_y.Callib();
	k_sleep(K_SECONDS(2));

	while(1)
	{
		
	}

	return 0;
}