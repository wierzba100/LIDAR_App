#ifndef SERVO_H
#define SERVO_H

#include <zephyr/drivers/pwm.h>

class Servo {
private:
    struct pwm_dt_spec pwm_servo;
    uint32_t current_pulse;

public:
    Servo(pwm_dt_spec pwm_servo_input);
    void Callib();
    void GoTo(uint32_t angle);
    void StepLeft();
    void StepRight();

};

#endif