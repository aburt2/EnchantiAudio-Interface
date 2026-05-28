#ifndef _ENCHANTI_SAI_H_
#define _ENCHANTI_SAI_H_

#include <Arduino.h>
#include "AudioTools.h"

// Standard Audio samplerates
enum STANDARD_SAMPLING_RATES {
    AUDIO_16K   = 16000,
    AUDIO_44_1K = 44100,
    AUDIO_48K   = 48000,
    AUDIO_96K   = 96000,
};

class enchantiSAI
{
    public:
        struct i2s_pin_config {
            int8_t DIN  = -1;
            int8_t DOUT = -1;
            int8_t BCLK = -1;
            int8_t WSEL = -1;
            int8_t MCLK = -1;
        };
        struct i2s_config {
            AudioInfo info;
            RxTxMode mode = RXTX_MODE;
            int i2s_port = 0;
            I2SSignalType signal_type = I2SSignalType::Digital; 
            I2SFormat i2s_format = I2SFormat::I2S_STD_FORMAT; 
            i2s_pin_config pins;
        };
        // I2S stream class
        I2SStream _stream;

        // Audio Info
        int channels;
        int sample_rate;
        int bps;

        // Initialise I2S interface
        bool begin(i2s_config *config) {
            // Get Audio information from config structure
            channels = config->info.channels;
            sample_rate = config->info.sample_rate;
            bps = config->info.bits_per_sample;

            /* Set up i2s stream configuration */
            auto cfg = _stream.defaultConfig(config->mode);
            cfg.copyFrom(config->info);

            // Setup I2S Format
            cfg.port_no = config->i2s_port;
            cfg.signal_type = config->signal_type;
            cfg.i2s_format = config->i2s_format;

            // Set up pins
            cfg.pin_ws      = config->pins.WSEL;
            cfg.pin_bck     = config->pins.BCLK;
            cfg.pin_mck     = config->pins.MCLK;
            switch (config->mode)
            {
            case RXTX_MODE:
                cfg.pin_data    = config->pins.DOUT;
                cfg.pin_data_rx = config->pins.DIN;
                break;
            case TX_MODE:
                cfg.pin_data    = config->pins.DOUT;
                cfg.pin_data_rx = -1;
                break;
            case RX_MODE:
                cfg.pin_data    = config->pins.DIN;
                cfg.pin_data_rx = -1;
                break;
            default:
                break;
            }

            // Start the stream
            _stream.begin(cfg);
        };

        // Check status of interface
        int available() {
            return _stream.available();
        }
        int peek() {
            return _stream.peek();
        }

        // Read Methods
        int read() {
            return _stream.read();
        }
        size_t readBytes(uint8_t *buffer, size_t size) {
            return _stream.readBytes(buffer, size);
        }

        // Write methods
        size_t write(const uint8_t *buffer, size_t size) {
            return _stream.write(buffer, size);
        }
};


#endif