

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mbed.h"

DigitalOut led1(LED1);

int main(int argc, char** argv) {

	LoWPANNDInterface mesh;
	NanostackRfPhyXX rf_phy;

	while(1) {
		led1 = !led1;
		wait(0.5);
	}
}

