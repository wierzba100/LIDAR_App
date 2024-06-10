#include "../hdr/servo.hpp"
#include <zephyr/logging/log.h>

#define PERIOD PWM_USEC(20000)
#define PWM_MIN_PULSE_WIDTH PWM_USEC(1000)
#define PWM_MAX_PULSE_WIDTH PWM_USEC(2000)
#define STEP_SIZE PWM_USEC(5.55)

LOG_MODULE_REGISTER(servo, LOG_LEVEL_INF);

Servo::Servo(pwm_dt_spec pwm_servo_input)
{
    pwm_servo = pwm_servo_input;
}

void Servo::Callib()
{
    int err;
    err = pwm_set_dt(&pwm_servo, PERIOD, PWM_MIN_PULSE_WIDTH);
        if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    current_pulse = PWM_MIN_PULSE_WIDTH;
    LOG_INF("Current pulse: %d", current_pulse);
}

void Servo::GoTo(uint32_t angle)
{
    current_pulse = ( (angle % 181) * (PWM_MAX_PULSE_WIDTH - PWM_MIN_PULSE_WIDTH) / 180) + PWM_MIN_PULSE_WIDTH;
    int err;
    err = pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
        if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    LOG_INF("Current pulse: %d", current_pulse);
}


void Servo::StepLeft()
{
    current_pulse = current_pulse + STEP_SIZE;
    if (current_pulse > PWM_MAX_PULSE_WIDTH)
    {
        current_pulse = PWM_MAX_PULSE_WIDTH;
    }
    int err;
    err = pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
        if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    LOG_INF("Current pulse: %d", current_pulse);
}

void Servo::StepRight()
{
    current_pulse = current_pulse - STEP_SIZE;
    if (current_pulse < PWM_MIN_PULSE_WIDTH)
    {
        current_pulse = PWM_MIN_PULSE_WIDTH;
    }
    int err;
    err = pwm_set_dt(&pwm_servo, PERIOD, current_pulse);
        if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    LOG_INF("Current pulse: %d", current_pulse);
}
