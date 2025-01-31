#include "BluetoothA2DPSource.h"
#include "Arduino.h"
#include "driver/i2s.h"

// Define I2S Pins (Match these with ESP32 #1 Output)
#define I2S_BCLK  14   // Bit Clock
#define I2S_LRC   15   // Word Select (Left/Right Clock)
#define I2S_DIN   22   // Data In from ESP32 #1

BluetoothA2DPSource a2dp_source;

// I2S Configuration
void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_in_num = I2S_DIN,
        .data_out_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

// Function to read I2S audio data from ESP32 #1
int32_t get_audio_data(int16_t *data, int32_t len) {
    size_t bytesRead;
    i2s_read(I2S_NUM_0, data, len * sizeof(int16_t), &bytesRead, portMAX_DELAY);
    return bytesRead / sizeof(int16_t);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Bluetooth A2DP Source...");
    
    setupI2S();  // Initialize I2S
    a2dp_source.start("ESP32-Audio", get_audio_data);  // Start Bluetooth audio transmission
}

void loop() {
    delay(1000);  // Keep running
}
