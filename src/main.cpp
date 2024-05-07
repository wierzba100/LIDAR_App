#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <memory>
#include "../hdr/servo.hpp"

int main(void)
{
	pwm_dt_spec pwm_servo_x = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_x));
	std::unique_ptr<Servo> servo_x = std::make_unique<Servo>(pwm_servo_x);

	pwm_dt_spec pwm_servo_y = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_y));
	std::unique_ptr<Servo> servo_y = std::make_unique<Servo>(pwm_servo_y);

	k_msleep(1000);

	servo_x->GoTo(0);
	servo_y->GoTo(90);
	k_msleep(5000);

	while(1)
	{
		servo_x->StepLeft();
		servo_y->StepLeft();
		k_msleep(500);
	}

	return 0;
}