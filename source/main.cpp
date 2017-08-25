

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "NanostackInterface.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed.h"
#include "rtos.h"

#include "NanostackRfPhyEfr32.h"
#include "sl_trng.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial usb(USBTX, USBRX, 115200);

LoWPANNDInterface mesh;
NanostackRfPhyEfr32 rf_phy;

Ticker ticker;

static Mutex SerialOutMutex;
void serial_out_mutex_wait() { SerialOutMutex.lock(); }
void serial_out_mutex_release() { SerialOutMutex.unlock(); }

static void blink() { led2 = !led2; }
void start_blinking() { ticker.attach(blink, 1.0); }

#define BLINK_CODE(t)                                                          \
  while (1) {                                                                  \
    led2 = !led2;                                                              \
    wait(t);                                                                   \
  }

int main(int argc, char **argv) {
  int res;

  usb.printf("Test print!\n");

  // Setup packet trace outputs
  mbed_trace_init();
  mbed_trace_print_function_set(trace_printer);
  mbed_trace_mutex_wait_function_set(serial_out_mutex_wait);
  mbed_trace_mutex_release_function_set(serial_out_mutex_release);

  #if 0
  // Initialise mesh
  res = mesh.initialize(&rf_phy);
  if (res < 0) {
    printf("Initialisation failed! %d\n", res);
    BLINK_CODE(0.1);
  }

  res = mesh.connect();
  if (res < 0) {
    printf("Connection failed! %d\n", res);
    BLINK_CODE(0.2);
  }

  ticker.attach(blink, 1.0);

  // Await IP allocation
  while (NULL == mesh.get_ip_address())
    Thread::wait(500);

  ticker.detach();
  ticker.attach(blink, 3.0);

  // Mesh connected
  printf("connected. IP = %s\n", mesh.get_ip_address());
#endif

  while (1) {
    led1 = !led1;
    usb.printf("Boop\r\n");
    wait(0.5);
  }
}
