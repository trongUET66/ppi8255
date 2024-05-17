#include "stm32f10x.h"

void delay(int n) {
    while(n--);
}

void send_data_to_8255(uint8_t data) {
    // Set data lines (PA1-PA8)
    GPIOA->ODR = (GPIOA->ODR & 0xFF00) | data;

    // Generate WR pulse
    GPIOA->ODR &= ~GPIO_ODR_ODR10; // WR low
    delay(100);
    GPIOA->ODR |= GPIO_ODR_ODR10;  // WR high
    delay(100);
}

void configure_8255_mode0() {
    // Set control word for Mode 0 (0x80 for all ports as output)
    uint8_t control_word = 0x80;

    // Set A0 and A1 for control register (A1 = 0, A0 = 0)
    GPIOA->ODR &= ~GPIO_ODR_ODR0 & ~GPIO_ODR_ODR1;
    // Set data lines for control word (no data lines needed for control word)
    GPIOA->ODR = (GPIOA->ODR & 0xFF00) | control_word;

    // Generate WR pulse
    GPIOA->ODR &= ~GPIO_ODR_ODR10; 
    delay(100);
}

int main() {
    // Cap clock cho port A và B
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Set thanh ghi ctrl (PA0, PA1, PB0 là ngõ ra push-pull)
    GPIOA->CRL = 0x33333333; // PA1-PA8 là ngõ ra push-pull
    GPIOA->ODR |= GPIO_ODR_ODR0; // Set PA0
    GPIOB->CRL = 0x33333333; // PB0 là ngõ ra push-pull
    GPIOB->ODR |= GPIO_ODR_ODR1; // Set PB1
    GPIOB->ODR |= GPIO_ODR_ODR0; // Set PB0 to logic high

    // Cau hinh 8255 o che do 0
    configure_8255_mode0();

    // Truyen thong tin tu 0 den 255
    for (uint8_t data = 0; data <= 255; data++) {
        send_data_to_8255(data);
        delay(50000); // Adjust the delay as needed
    }

    while (1) {
       
    }
}
