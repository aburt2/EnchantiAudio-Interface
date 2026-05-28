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
        struct i2s_config {
            AudioInfo info;
            I2SConfig i2s_cfg;
        };
        // I2S stream class
        I2SStream *_stream = NULL;

        // Config
        I2SConfig cfg;

        // Audio Info
        AudioInfo info;

        // Initialise I2S interface
        bool begin(i2s_config *config) {
            // Set i2s Stream
            _stream = new I2SStream;

            // Get the config and audio info from the structure
            cfg = config->i2s_cfg;
            info = config->info;

            // Update config with audio info
            cfg.copyFrom(info);

            // Start the stream
            _stream->begin(cfg);
        };

        // Check status of interface
        int available() {
            return _stream->available();
        }
        int peek() {
            return _stream->peek();
        }

        // Read Methods
        int read() {
            return _stream->read();
        }
        size_t readBytes(uint8_t *buffer, size_t size) {
            return _stream->readBytes(buffer, size);
        }

        // Write methods
        size_t write(const uint8_t *buffer, size_t size) {
            return _stream->write(buffer, size);
        }
};


#endif