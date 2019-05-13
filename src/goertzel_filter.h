#ifndef GOERTZEL_FILTER_H
#define GOERTZEL_FILTER_H

#include <vector>

class GoertzelFilter
{
public:
    /** @brief Filter to detect the presence of a target frequency in a wav file buffer.
     *
     *  @param target_frequency to detect in the sample.
     *  @param block_size of the filter, determines bin width. (ex: 8 kHz sample rate and block_size = 100 gives 80 Hz bin width.)
     *  @param sample_rate of the buffer, in Hz.
     *  @param target_threshold determines the result's cutoff point when detecting a frequency.
     *
     *  @note This was heavily influenced by the wonderful article at: https://www.embedded.com/design/configurable-systems/4024443/The-Goertzel-Algorithm
     */

    GoertzelFilter(double target_frequency, double block_size, double sample_rate, double target_threshold);

    /** @brief Run filter setup on buffer for detection.
     *  @param buffer of the sample.
     *  @returns true if the target frequency was detected, otherwise false.
     */
    bool detect_frequency(std::vector<double>& buffer) const;

    /** @brief Run filter setup on buffer for detection.
     *  @param buffer of the sample.
     *  @returns magnitude of the detection filter.
     */
    double filter_magnitude(std::vector<double>& buffer) const;

private:
    double K;
    double W;
    double cosine;
    double coefficient;
    double bins;
    double threshold;
};

#endif
