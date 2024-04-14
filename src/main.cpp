#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include "../hdr/servo.hpp"

int main(void)
{
	pwm_dt_spec pwm_servo_x = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo_x));
	Servo Servo_x(pwm_servo_x);

	while(1)
	{
		Servo_x.GoTo(0);
		k_sleep(K_SECONDS(1U));

		Servo_x.GoTo(45);
		k_sleep(K_SECONDS(1U));

		Servo_x.GoTo(90);
		k_sleep(K_SECONDS(1U));

		Servo_x.GoTo(135);
		k_sleep(K_SECONDS(1U));

		Servo_x.GoTo(180);
		k_sleep(K_SECONDS(1U));

		Servo_x.GoTo(90);
		k_sleep(K_SECONDS(5U));

		Servo_x.StepRight();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepRight();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepRight();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepRight();
		k_sleep(K_SECONDS(5U));

		Servo_x.StepLeft();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepLeft();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepLeft();
		k_sleep(K_SECONDS(1U));
		Servo_x.StepLeft();
		k_sleep(K_SECONDS(5U));
	}

	return 0;
}