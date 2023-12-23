#include <ESP8266WiFi.h>

const uint8_t UART_NO = 0; // We'll use UART0
const uint32_t SAMPLE_RATE = 200000; // 200 kHz

volatile bool sampleFlag = false;

void setup() {
 // Turn off Wifi to save power
 WiFi.mode(WIFI_OFF);
 WiFi.forceSleepBegin();
 delay(1);

 // Set up the ADC
 system_adc_set_vref(ADC_VREF_3_3); // Set ADC reference voltage
 system_adc_set_width(ADC_WIDTH_BIT_12); // Set ADC resolution

 // Set up DMA
 dma_set_trigger_type(DMA_TRIGGER_ADC); // Use ADC as DMA trigger source
 dma_set_buf_size(1024); // Set DMA buffer size
 dma_set_max_count(SAMPLE_RATE); // Set maximum number of ADC samples
 dma_set_auto_write_mode(true); // Use DMA to write data automatically
 dma_attach_interrupt(DMA_ATTACH_ADC); // Attach interrupt to ADC DMA operation

 // Set up UART
 Serial.begin(2000000);
 system_uart_de_attach(); // Disable UART

 // Enable ADC
 system_adc_enable();

 // Enable DMA
 dma_init();
 dma_enable();

 // Enable interrupt
 system_intr_enable();
 ets_intr_lock(); // Disable interrupt
}

void loop() {
 if (sampleFlag) {
    sampleFlag = false;
    uint32_t data = dma_read_buf_data(); // Read data from DMA buffer
    Serial.write(data & 0xFF); // Send low byte
    Serial.write((data >> 8) & 0xFF); // Send high byte
 }
}

// DMA interrupt handler
void ICACHE_RAM_ATTR onDMAInterrupt() {
 if (dma_is_buf_overflow()) {
    dma_reset_buf_overflow();
 }
 sampleFlag = true;
}

extern "C" {
 void __dma_isr() {
    onDMAInterrupt();
 }
}
