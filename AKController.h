#ifndef AKCONTROLLER_H_
#define AKCONTROLLER_H_

#include "mbed.h"
#include "m3pi.h"

class AKController {
public:
    AKController();
    virtual ~AKController();

    void init();

private:
    void rx_pc();
    void rx_wifly();

    Serial pc;
    Serial wifly;
    m3pi pi;
};

#endif /* AKCONTROLLER_H_ */
