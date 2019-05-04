#include "audio_filters.h"

#include <cmath>

AudioFilters::AudioFilters()
{

}

bool AudioFilters::detect_frequency_goertzel(double target_frequency, double target_threshold, double sample_rate, double block_size, std::vector<int> buffer)
{
    // Sanity check, we must be able to process as many samples as there are bins.
    if (buffer.size() < block_size) {
        return false;
    }

    // Set up required constants. // TODO: Move these into a class of their own so that a filter can be set up and pre-process these.
    const int K = static_cast<int>(0.5 + ((block_size * target_frequency) / sample_rate));
    const double W = ((2.0 * M_PI) / block_size) * K;
    const double cosine = cos(W);
    const double sine = sin(W);
    const double coefficient = 2.0 * cosine;

    // Intra-processing buffers.
    int64_t q_0 = 0;
    int64_t q_1 = 0;
    int64_t q_2 = 0;

    // Processing block.
    for (int refine = 0; refine < block_size; refine++) {
        q_0 = coefficient * q_1 - q_2 + buffer.back();
        q_2 = q_1;
        q_1 = q_0;
        buffer.pop_back();
    }

    // Detection Block
    int64_t magnitude = pow(q_1, 2) + pow(q_2, 2) - q_1 * q_2 * coefficient;
    magnitude = sqrt(magnitude);

    return magnitude > static_cast<int64_t>(target_threshold);
}
