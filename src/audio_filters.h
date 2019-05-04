#ifndef AUDIO_FILTERS_H
#define AUDIO_FILTERS_H

#include <vector>

class AudioFilters
{
public:
    AudioFilters();

    /**
     * @brief Detect the presence of a target frequency in a wav file buffer.
     *
     * @param target_frequency to detect in the sample.
     * @param target_threshold determines the result's cutoff point when detecting a frequency.
     * @param sample_rate of the buffer, in Hz.
     * @param block_size of the filter, determines bin width. (ex: 8 kHz sample rate and block_size = 100 gives 80 Hz bin width.)
     * @param buffer of the sample.
     *
     * @returns true if the target frequency was detected, otherwise false.
     *
     * @note This was heavily influenced by the wonderful article at: https://www.embedded.com/design/configurable-systems/4024443/The-Goertzel-Algorithm
     */
    bool detect_frequency_goertzel(double target_frequency, double target_threshold, double sample_rate, double block_size, std::vector<int> buffer);
};

#endif // AUDIO_FILTERS_H
