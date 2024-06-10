#ifndef SERVO_H
#define SERVO_H

#include <zephyr/drivers/pwm.h>

#define SERVO_X_RANGE 181
#define SERVO_Y_RANGE 91

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