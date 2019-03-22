#include <pigpio.h>
#include <iostream>
#using namespace std; #Commented out to see what happens JR


int main(){
    gpioInitialise();
    xfer.control = (0x0A<<16) | 0x305; // Set I2C slave Address to 0x0A
    int status = bscXfer(&xfer);
    if (status >= 0)
    {
        xfer.rxCnt = 0;
        while(1){
            bsc_xfer_t xfer;
            if (xfer.rxCnt > 0){
                cout << xfer.rxBuf;
            }
       }    
    }

    return 1;
} 
