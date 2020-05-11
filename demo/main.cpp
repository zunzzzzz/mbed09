#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/
DigitalOut redLED(LED1);
DigitalOut blueLED(LED3);
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled3(LED3,"myled3");
Serial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
double x, y;

int main() {
    redLED = 1;
    blueLED = 1;
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];
    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = pc.getc();
            if (recv == '\r') {
                pc.printf("\r\n");
                break;
            }

            buf[i] = pc.putc(recv);
        }

        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        pc.printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;


    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256];
    char strings[20];

    sprintf(strings, "/myled1/write 0");
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    wait(1);

    sprintf(strings, "/myled1/write 1");
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    wait(1);

    sprintf(strings, "/myled3/write 0");
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    wait(1);

    sprintf(strings, "/myled3/write 1");
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    wait(1);
    if (success) {
        out->putData("Blink red and blue LED respectively");
    } 
}