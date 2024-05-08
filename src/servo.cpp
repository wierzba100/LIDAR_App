#include "../hdr/servo.hpp"

#define PERIOD PWM_USEC(20000)
#define PWM_MIN_PULSE_WIDTH PWM_USEC(500)
#define PWM_MAX_PULSE_WIDTH PWM_USEC(2500)
#define STEP_SIZE PWM_USEC(11.11)

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
    current_pulse = ( (angle % 181) * (PWM_MAX_PULSE_WIDTH - PWM_MIN_PULSE_WIDTH) / 180) + PWM_MIN_PULSE_WIDTH;
    pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
}


void Servo::StepLeft()
{
    current_pulse = current_pulse + STEP_SIZE;
    if (current_pulse > PWM_MAX_PULSE_WIDTH)
    {
        current_pulse = PWM_MAX_PULSE_WIDTH;
    }
    pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
}

void Servo::StepRight()
{
    current_pulse = current_pulse - STEP_SIZE;
    if (current_pulse < PWM_MIN_PULSE_WIDTH)
    {
        current_pulse = PWM_MIN_PULSE_WIDTH;
    }
    pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
}
