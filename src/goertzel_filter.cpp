#include "goertzel_filter.h"

#include <cmath>

// Set up constants for filter.
GoertzelFilter::GoertzelFilter(double target_frequency, double block_size, double sample_rate, double target_threshold)
    : K(0.5 + ((block_size * target_frequency) / sample_rate))
    , W(((2.0 * M_PI) / block_size) * K)
    , cosine(cos(W))
    , coefficient(2.0 * cosine)
    , bins(block_size)
    , threshold(target_threshold)
{
}

bool GoertzelFilter::detect_frequency(std::vector<int> buffer) const
{
    // Sanity check, we must be able to process as many samples as there are bins.
    if (buffer.size() < this->bins) {
        return false;
    }

    // Intra-processing buffers.
    int64_t q_0 = 0;
    int64_t q_1 = 0;
    int64_t q_2 = 0;

    // Processing block.
    for (int refine_step = 0; refine_step < this->bins; refine_step++) {
        q_0 = coefficient * q_1 - q_2 + buffer.at(refine_step);
        q_2 = q_1;
        q_1 = q_0;
    }

    // Detection Block
    int64_t magnitude = pow(q_1, 2) + pow(q_2, 2) - q_1 * q_2 * coefficient;
    magnitude = sqrt(magnitude);

    return magnitude > static_cast<int64_t>(this->threshold);
}
