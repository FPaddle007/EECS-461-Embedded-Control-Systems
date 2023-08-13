#include "gpio.h"
const uint8_t LED[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const uint8_t LED_BASE[16] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
const uint8_t DIP[16] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 1, 2, 3, 17};
const uint8_t DIP_BASE[16] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3};
/* base: 0-4 corresponding to gpio/port groups A-E */
GPIO_Type * gpio_base_ptrs[5] = GPIO_BASE_PTRS;
PORT_Type * port_base_ptrs[5] = PORT_BASE_PTRS;
/******************************************************************************
 * Function: initGPDI
 * Description: Initialize a general purpose input based on port base and pin
 ******************************************************************************/
void initGPDI(const uint8_t base, const uint8_t pin)
{
 /* Set the pin direction to input */
 gpio_base_ptrs[base]->PDDR |= 0b0 << pin;
 /* Set the pin configuration register */
 /* Pin Mux Alt1 for GPIO */
 port_base_ptrs[base]->PCR[pin] = PORT_PCR_MUX(0b001);
 /* All other bitfields to 0 */
 /* Internal pulldown or input filter are optional */
}
/******************************************************************************
 * Function: initGPDO
 * Description: Initialize a general purpose output based on port base and pin
 ******************************************************************************/
void initGPDO(const uint8_t base, const uint8_t pin)
{
 /* Set the pin direction to output */
 gpio_base_ptrs[base]->PDDR |= 0b1 << pin;
 /* Pin Mux Alt1 for GPIO */
 /* Pin Mux Alt1 for GPIO */
 port_base_ptrs[base]->PCR[pin] = PORT_PCR_MUX(0b001);
 /* All other bitfields to 0 */
}
/******************************************************************************
 * Function: readGPIO
 * Description: Read the digital 1/0 from GPIO input register
 ******************************************************************************/
uint8_t readGPIO(const uint8_t base, const uint8_t pin)
{
 return ((gpio_base_ptrs[base]->PDIR)>>pin)&0b1;
}
/******************************************************************************
 * Function: writeGPIO
 * Description: Write a digital 1/0 to a GPIO output
 ******************************************************************************/
void writeGPIO(const uint8_t base, const uint8_t pin, const uint8_t val)
{
 (gpio_base_ptrs[base]->PDOR) &= ~(0b1<<pin);
 (gpio_base_ptrs[base]->PDOR) |=(val<<pin);
 return;
}
