#ifndef AKCONTROLLER_H_
#define AKCONTROLLER_H_

#include "mbed.h"
#include "m3pi.h"
#include "rtos.h"

class AKController {

public:
    AKController();
    virtual ~AKController();

    void run();

private:
    static void rx_pc_threaded(const void* self);
    void rx_pc();
    static void rx_wifly_threaded(const void* self);
    void rx_wifly();
    void accel_step();
    float accel_step_val(float val, float tgt);
    bool on_tgt_speed();
    static void update_motors_forever_threaded(const void* self);
    void update_motors_forever();

    Serial pc;
    Serial wifly;
    m3pi pi;

    float left_motor_speed;
    float right_motor_speed;
    float tgt_left_motor_speed;
    float tgt_right_motor_speed;
};

#endif /* AKCONTROLLER_H_ */
