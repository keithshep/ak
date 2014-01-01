#include "AKController.h"

static AKController ak;

int main() {
    wait(0.5);
    
    ak.init();
    
    return 0;
}
