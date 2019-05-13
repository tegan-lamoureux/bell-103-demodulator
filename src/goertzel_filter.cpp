#include "goertzel_filter.h"

#include <cmath>

// Set up constants for filter.
GoertzelFilter::GoertzelFilter(double target_frequency, double block_size, double sample_rate, double target_threshold)
{
    K = ((block_size * target_frequency) / sample_rate);
    W = ((2.0 * M_PI * K) / block_size);
    cosine = cos(W);
    coefficient = 2.0 * cosine;
    bins = block_size;
    threshold = target_threshold;
}

double GoertzelFilter::filter_magnitude(std::vector<double>& buffer) const
{
    // Sanity check, we must be able to process as many samples as there are bins.
    if (buffer.size() < this->bins) {
        return false;
    }

    // Intra-processing buffers.
    double q_0 = 0;
    double q_1 = 0;
    double q_2 = 0;

    // Processing block.
    for (int refine_step = 0; refine_step < this->bins; refine_step++) {
        q_0 = coefficient * q_1 - q_2 + buffer.at(refine_step);
        q_2 = q_1;
        q_1 = q_0;
    }

    // Detection Block
    double magnitude = pow(q_1, 2) + pow(q_2, 2) - q_1 * q_2 * coefficient;
    magnitude = sqrt(magnitude);
    magnitude = magnitude / (bins / 2); // Scale magnitude to 1.

    return magnitude;
}

bool GoertzelFilter::detect_frequency(std::vector<double>& buffer) const
{
    return this->filter_magnitude(buffer) > this->threshold;
}
