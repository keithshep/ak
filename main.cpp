#include "mbed.h"
#include "m3pi.h"

void echo_wifly_tx(void);
void forward_pc_tx(void);

Serial pc(USBTX, USBRX);
Serial wifly(p28, p27); // tx rx

m3pi pi;

int main() {
    wait(0.5);
    pi.stop();
    
    pc.printf("\n== AK ==\n");
    pc.attach(&forward_pc_tx);
    wifly.attach(&echo_wifly_tx);
    
    return 0;
}

void echo_wifly_tx() {
    while(wifly.readable()) {
        int c = wifly.getc();
        pc.putc(c);
        switch(c) {
            case 'i':
                pi.forward(0.5);
                break;
            case 'j':
                pi.left(0.2);
                break;
            case 'l':
                pi.right(0.2);
                break;
            case ',':
                pi.backward(0.2);
                break;
            case 'k':
                pi.stop();
                break;
            case '\n':
            case '\r':
            case '\b':
                break;
            default:
                pi.stop();
                break;
        }
    }
}

void forward_pc_tx() {
    // forward anything we hear from the PC on to the wifly
    while(pc.readable()) {
        wifly.putc(pc.getc());
    }
}

