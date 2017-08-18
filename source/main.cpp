

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mbed.h"
#include "rtos.h"
#include "NanostackInterface.h"
#include "mbed-trace/mbed_trace.h"

#include "NanostackRfPhyEfr32.h"
#include "sl_trng.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);

LoWPANNDInterface mesh;
NanostackRfPhyEfr32 rf_phy;

Ticker ticker;

static Mutex SerialOutMutex;
void serial_out_mutex_wait() {
    SerialOutMutex.lock();
}
void serial_out_mutex_release() {
    SerialOutMutex.unlock();
}

static void blink() {
    led1 = !led1;
}
void start_blinking() {
    ticker.attach(blink, 1.0);
}

int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen ) {
    //trng_t trng_obj;
    //trng_init(&trng_obj);
    //int ret = trng_get_bytes(&trng_obj, output, len, olen);
    //trng_free(&trng_obj);
	//return ret;
	return -1;
}

int main(int argc, char** argv) {

	//sl_trng_init(TRNG0);

	// Setup packet trace outputs
	mbed_trace_init();
    mbed_trace_print_function_set(trace_printer);
    mbed_trace_mutex_wait_function_set( serial_out_mutex_wait );
    mbed_trace_mutex_release_function_set( serial_out_mutex_release );

	ticker.attach(blink, 0.5);

	// Initialise mesh
	mesh.initialize(&rf_phy);
    int error = mesh.connect();
    if (error) {
        printf("Connection failed! %d\n", error);
        return error;
    }

	// Await IP allocation
	while (NULL == mesh.get_ip_address())
		Thread::wait(500);

	ticker.detach();
	ticker.attach(blink, 1.0);

	// Mesh connected
	printf("connected. IP = %s\n", mesh.get_ip_address());

	while(1) {
		
	}
}

