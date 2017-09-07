

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "mbed-trace/mbed_trace.h"
#include "mbed.h"
#include "mbed_mem_trace.h"
#include "mbed_stats.h"
#include "rtos.h"

#include "NanostackInterface.h"
#include "NanostackRfPhyEfr32.h"

#include "sl_trng.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial usb(USBTX, USBRX, 115200);

LoWPANNDInterface *mesh;
NanostackRfPhyEfr32 *rf_phy;

static Mutex SerialOutMutex;
void serial_out_mutex_wait() { SerialOutMutex.lock(); }
void serial_out_mutex_release() { SerialOutMutex.unlock(); }

// Runtime stats monitoring
// See:
// https://docs.mbed.com/docs/mbed-os-handbook/en/latest/advanced/runtime_stats/#heap-stats
// https://docs.mbed.com/docs/mbed-os-handbook/en/5.2/advanced/runtime_mem_trace/
void mem_stats() {
  // Heap
  mbed_stats_heap_t heap_stats;
  mbed_stats_heap_get(&heap_stats);
  printf("Heap info: \r\n");
  printf("  - allocated: %lu\r\n", heap_stats.reserved_size);
  printf("  - used: %lu\r\n", heap_stats.current_size);
  printf("  - maximum: %lu\r\n", heap_stats.max_size);
  printf("  - num allocations: %lu\r\n", heap_stats.alloc_cnt);
  printf("  - num failures: %lu\r\n", heap_stats.alloc_fail_cnt);

  // Stacks
  int cnt = osThreadGetCount();
  mbed_stats_stack_t *stats =
      (mbed_stats_stack_t *)malloc(cnt * sizeof(mbed_stats_stack_t));
  if (stats == NULL) {
    printf("Failed to allocate memory for stack statistics\n");
    return;
  }

  printf("Stack info: \r\n");
  cnt = mbed_stats_stack_get_each(stats, cnt);
  for (int i = 0; i < cnt; i++) {
    printf("  - thread: 0x%X, allocated: %u, used: %u\r\n", stats[i].thread_id,
           stats[i].reserved_size, stats[i].max_size);
  }
  free(stats);
}

int main(int argc, char **argv) {
  int res;

  mbed_mem_trace_set_callback(mbed_mem_trace_default_callback);

  osThreadId mainThread = osThreadGetId();

  printf("\nHello from MBedOS5!\n");

  mem_stats();

//__asm__("BKPT #1");

// Setup packet trace outputs
#if MBED_CONF_APP_TRACE
  mbed_trace_init();
  mbed_trace_print_function_set(trace_printer);
  mbed_trace_mutex_wait_function_set(serial_out_mutex_wait);
  mbed_trace_mutex_release_function_set(serial_out_mutex_release);
#endif

  printf("Creating mesh interface\n");
  mesh = new LoWPANNDInterface();
  mem_stats();

  printf("Creating nanostack instance\n");
  rf_phy = new NanostackRfPhyEfr32();
  mem_stats();

#if 0
  // Initialise mesh
  res = mesh->initialize(rf_phy);
  if (res < 0) {
    printf("Mesh initialisation failed! %d\n", res);
    while(1);
  }
#endif

#if 0
  res = mesh.connect();
  if (res < 0) {
    printf("Connection failed! %d\n", res);
  }

  // Await IP allocation
  while (NULL == mesh.get_ip_address())
    Thread::wait(500);

  ticker.detach();
  // Mesh connected
  printf("connected. IP = %s\n", mesh.get_ip_address());
#endif

  printf("Boot OK!\r\n");

  while (1) {
    led1 = !led1;

    wait(0.5);
  }
}
