#ifndef AKCONTROLLER_H_
#define AKCONTROLLER_H_

#include "mbed.h"
#include "m3pi.h"

class AKController {

public:
    AKController();
    virtual ~AKController();

    void run();

private:
    void rx_pc();
    void rx_wifly();
    void accel_step();
    float accel_step_val(float val, float tgt);
    bool on_tgt_speed();

    Serial pc;
    Serial wifly;
    m3pi pi;

    Ticker speed_change_ticker;
    float left_motor_speed;
    float right_motor_speed;
    float tgt_left_motor_speed;
    float tgt_right_motor_speed;
};

#endif /* AKCONTROLLER_H_ */
