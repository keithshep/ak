#include <stdint.h>
#include <math.h>

#include "AKController.h"

#define ACCEL_UNITS_PER_SEC 0.75f
#define ACCEL_TIMESLICE_MS 100

// we use an even value so that we can have an exact zero
#define MAX_MOTOR_WIRE_VAL 254
static float to_motor_speed(int wire_speed) {
    int half_max = MAX_MOTOR_WIRE_VAL / 2;
    return (wire_speed - half_max) / (float)half_max;
}

AKController::AKController()
: pc(USBTX, USBRX), wifly(p28, p27), pi(),
  left_motor_speed(0.0f), right_motor_speed(0.0f),
  tgt_left_motor_speed(0.0f), tgt_right_motor_speed(0.0f) {
}

AKController::~AKController() {
}

void AKController::run() {
    this->pc.printf("\n== AK ==\n");

    Thread rx_pc_thread(AKController::rx_pc_threaded, this);
    Thread rx_wifly_thread(AKController::rx_wifly_threaded, this);
    this->update_motors_forever();
}

void AKController::rx_pc_threaded(const void* self) {
    ((AKController*)self)->rx_pc();
}

void AKController::rx_pc() {
    // forward anything we hear from the PC on to the wifly
    while(true) {
        this->wifly.putc(this->pc.getc());
    }
}

void AKController::rx_wifly_threaded(const void* self) {
    ((AKController*)self)->rx_wifly();
}

void AKController::rx_wifly() {
    bool alreadyHeardZeroZero = false;

    // yay we have a data connection. read instructions forever
    while(true) {
        // we always expect a left motor value followed by right
        int c1 = this->wifly.getc();
        if(!alreadyHeardZeroZero) {
            this->pc.putc(c1);
        }

        int c2 = this->wifly.getc();
        if(!alreadyHeardZeroZero) {
            this->pc.putc(c2);
        }

        float left_motor_val = to_motor_speed(c1);
        float right_motor_val = to_motor_speed(c2);

        if(!alreadyHeardZeroZero) {
            alreadyHeardZeroZero = left_motor_val == 0.0f && right_motor_val == 0.0f;
        }

        if(alreadyHeardZeroZero) {
            this->tgt_left_motor_speed = left_motor_val;
            this->tgt_right_motor_speed = right_motor_val;
        }
    }
}

void AKController::accel_step() {
    this->left_motor_speed = this->accel_step_val(
            this->left_motor_speed,
            this->tgt_left_motor_speed);
    this->right_motor_speed = this->accel_step_val(
            this->right_motor_speed,
            this->tgt_right_motor_speed);

    // special case for when we're stopping. In this case we want the robot to
    // move straight (otherwise you get these strange quick turns toward the
    // end of the stop)
    if(this->tgt_left_motor_speed == 0.0f && this->tgt_right_motor_speed == 0.0f &&
       (this->left_motor_speed * this->right_motor_speed >= 0.0f)) {
        if(fabsf(this->left_motor_speed) < fabsf(this->right_motor_speed)) {
            this->right_motor_speed = this->left_motor_speed;
        } else {
            this->left_motor_speed = this->right_motor_speed;
        }
    }

    this->pi.left_motor(this->left_motor_speed);
    this->pi.right_motor(this->right_motor_speed);
}

float AKController::accel_step_val(float val, float tgt) {
    float step_size = ACCEL_UNITS_PER_SEC * ACCEL_TIMESLICE_MS / 1000.0f;
    if(val > tgt) {
        val -= step_size;
        if(val < tgt) {
            val = tgt;
        }
    } else if(val < tgt) {
        val += step_size;
        if(val > tgt) {
            val = tgt;
        }
    }

    return val;
}

bool AKController::on_tgt_speed() {
    return
            this->left_motor_speed == this->tgt_left_motor_speed
            && this->right_motor_speed == this->tgt_right_motor_speed;
}

void AKController::update_motors_forever_threaded(const void* self) {
    ((AKController*)self)->update_motors_forever();
}

void AKController::update_motors_forever() {
    while(true) {
        if(!this->on_tgt_speed()) {
            this->accel_step();
        }
        Thread::wait(ACCEL_TIMESLICE_MS);
    }
}
