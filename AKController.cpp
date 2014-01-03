#include "AKController.h"

#define ACCEL_UNITS_PER_SEC 0.75f
#define ACCEL_TIMESLICE_MS 100

AKController::AKController()
: pc(USBTX, USBRX), wifly(p28, p27), pi(),
  speed_change_ticker(),
  left_motor_speed(0.0f), right_motor_speed(0.0f),
  tgt_left_motor_speed(0.0f), tgt_right_motor_speed(0.0f) {
}

AKController::~AKController() {
}

void AKController::run() {
    this->pc.printf("\n== AK ==\n");
    this->pc.attach(this, &AKController::rx_pc);
    this->wifly.attach(this, &AKController::rx_wifly);
    this->pi.stop();
}

void AKController::rx_pc() {
    // forward anything we hear from the PC on to the wifly
    while(this->pc.readable()) {
        this->wifly.putc(pc.getc());
    }
}

void AKController::rx_wifly() {
    bool update_in_progress = !this->on_tgt_speed();
    while(this->wifly.readable()) {
        int c = this->wifly.getc();
        this->pc.putc(c);
        switch(c) {
            // forward
            case 'i':
                this->tgt_left_motor_speed = 0.75f;
                this->tgt_right_motor_speed = 0.75f;
                break;

            // left
            case 'j':
                this->tgt_left_motor_speed = 0.2f;
                this->tgt_right_motor_speed = -0.2f;
                break;

            // right
            case 'l':
                this->tgt_left_motor_speed = -0.2f;
                this->tgt_right_motor_speed = 0.2f;
                break;

            // back
            case ',':
                this->tgt_left_motor_speed = -0.5f;
                this->tgt_right_motor_speed = -0.5f;
                break;

            // stop
            case 'k':
                this->tgt_left_motor_speed = 0.0f;
                this->tgt_right_motor_speed = 0.0f;
                break;

            default:
                break;
        }
    }

    // if an update is in progress just let it continue
    if(!update_in_progress && !this->on_tgt_speed()) {
        this->accel_step();
        if(!this->on_tgt_speed()) {
            this->speed_change_ticker.attach_us(
                    this,
                    &AKController::accel_step,
                    ACCEL_TIMESLICE_MS * 1000);
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
    this->pi.left_motor(this->left_motor_speed);
    this->pi.right_motor(this->right_motor_speed);
    if(this->on_tgt_speed()) {
        this->speed_change_ticker.detach();
    }
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
