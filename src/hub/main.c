/**
 * main (entrypoint) implementation
 */
#include "sam.h"
#include "system_samd51.h"

#include "sys/init.h"

void loop(void) {
    //implement
}

int main(void) {
    SystemInit();
    AppInit();
    for(;;) loop();
}
