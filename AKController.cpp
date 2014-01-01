#include "AKController.h"

AKController::AKController() : pc(USBTX, USBRX), wifly(p28, p27), pi() {
}

AKController::~AKController() {
}

void AKController::init() {
    this->pc.printf("\n== AK ==\n");
    this->pc.attach(this, &AKController::rx_pc);
    this->wifly.attach(this, &AKController::rx_wifly);
}

void AKController::rx_pc() {
    // forward anything we hear from the PC on to the wifly
    while(this->pc.readable()) {
        this->wifly.putc(pc.getc());
    }
}

void AKController::rx_wifly() {
    while(this->wifly.readable()) {
        int c = this->wifly.getc();
        this->pc.putc(c);
        switch(c) {
            case 'i':
                this->pi.forward(0.5);
                break;
            case 'j':
                this->pi.left(0.2);
                break;
            case 'l':
                this->pi.right(0.2);
                break;
            case ',':
                this->pi.backward(0.2);
                break;
            case 'k':
                this->pi.stop();
                break;
            case '\n':
            case '\r':
            case '\b':
                break;
            default:
                this->pi.stop();
                break;
        }
    }
}
