#include "smm_delay.h"

void usDelay(uint32_t delay)
{
	uint64_t k = 0;
	for(k=0; k<20*delay; k++)
		__NOP();
}
/****************************************/
void msDelay(uint32_t delay)
{
	uint64_t k = 0;
	for(k=0; k<20000*delay; k++)
		__NOP();
}
