#include "../hdr/servo.hpp"

#define PERIOD PWM_USEC(20000)
#define PWM_MIN_PULSE_WIDTH PWM_USEC(1000)
#define PWM_MAX_PULSE_WIDTH PWM_USEC(2000)
#define STEP_SIZE PWM_USEC(5.55)

Servo::Servo(pwm_dt_spec pwm_servo_input)
{
    pwm_servo = pwm_servo_input;
}

void Servo::Callib()
{
    pwm_set_dt(&pwm_servo, PERIOD, PWM_MIN_PULSE_WIDTH);
    current_pulse = PWM_MIN_PULSE_WIDTH;
}

void Servo::GoTo(uint32_t angle)
{
    uint32_t pulse_width = (angle * (PWM_MAX_PULSE_WIDTH - PWM_MIN_PULSE_WIDTH) / 180) + PWM_MIN_PULSE_WIDTH;
    pwm_set_dt(&pwm_servo, PERIOD, pulse_width);
    current_pulse = pulse_width;
}


void Servo::StepLeft()
{
    current_pulse = current_pulse + STEP_SIZE;
    pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
}

void Servo::StepRight()
{
    current_pulse = current_pulse - STEP_SIZE;
    pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
}
