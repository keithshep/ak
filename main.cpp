#include "mbed.h"
#include "m3pi.h"

m3pi pi;

int main() {
    wait(0.5);
    
    pi.forward(0.5);
    wait (0.5);
    pi.left(0.5);
    wait (0.5);
    pi.backward(0.5);
    wait (0.5);
    pi.right(0.5);
    wait (0.5);
    
    pi.stop();
}
