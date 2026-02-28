#include "wb32f10x_it.h"

static volatile uint32_t systick_ms;

void NMI_Handler(void) {}

void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void)  { while (1); }
void UsageFault_Handler(void) { while (1); }

void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}

void SysTick_Handler(void)
{
  systick_ms++;
}

uint32_t get_tick(void)
{
  return systick_ms;
}

void delay_ms(uint32_t ms)
{
  uint32_t start = systick_ms;
  while ((systick_ms - start) < ms);
}
