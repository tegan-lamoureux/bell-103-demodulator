#include "gtest/gtest.h"

#include "goertzel_filter.h"
#include <sndfile.h>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

namespace {

// Can instantiate goertzel filter.
TEST(AudioFilterTests, can_use_libsndfile) {
    bool got_this_far = true;

    GoertzelFilter filter(1, 1, 1, 1);

    ASSERT_TRUE(got_this_far);
}


// Can detect a frequency using the filter. Relies on LibSndFileTests.can_create_and_save_wav_file
// for the test data.
TEST(AudioFilterTests, can_detect_frequency_from_single_frequency_source) {
    SNDFILE* file;
    SF_INFO sfinfo;
    vector<double> filter_samples;
    const double sample_rate = 44100.0;
    const double block_size = 320.0;
    const double target_frequency = 350.0;

    sfinfo.samplerate	= static_cast<int>(sample_rate);
    sfinfo.frames		= (static_cast<int>(sample_rate) * 4);
    sfinfo.channels		= 1;
    sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24);

    file = sf_open ("sine_350hz.wav", SFM_READ, &sfinfo);

    ASSERT_NE(nullptr, file); // Check the file opened.

    sf_count_t samples_read = 0;
    double samples[1000];

    samples_read = sf_read_double(file, samples, 1000);
    filter_samples.assign(samples, samples+1000);


    // Test detection at target frequency
    GoertzelFilter filter_at_target_freq(target_frequency, block_size, sample_rate, 0);
    ASSERT_TRUE(filter_at_target_freq.detect_frequency(filter_samples));

    // Prove bin width.
    // 44.1 kHz and block size of 320 give bin width of 137 Hz.
    const double bin_width = (sample_rate / block_size);
    const int bin_of_target_frequency = static_cast<int>((target_frequency / bin_width) + 0.5);

    // Filter at end of target freq's bin.
    GoertzelFilter filter_same_bin_as_target_freq_1((bin_of_target_frequency * bin_width) - (bin_width * 0.1), block_size, sample_rate, 0);
    // Filter at start of target freq's bin.
    GoertzelFilter filter_same_bin_as_target_freq_2(((bin_of_target_frequency + 1) * bin_width) - (bin_width * 0.9), block_size, sample_rate, 0);

    // Check:
    // 1. Two test filters are in the same bin (have same magnitude detected).
    // 2. Each individual filter is in the target freq's bin.
    ASSERT_DOUBLE_EQ(filter_same_bin_as_target_freq_1.filter_magnitude(filter_samples), filter_same_bin_as_target_freq_2.filter_magnitude(filter_samples));
    ASSERT_DOUBLE_EQ(filter_same_bin_as_target_freq_1.filter_magnitude(filter_samples), filter_at_target_freq.filter_magnitude(filter_samples));
    ASSERT_DOUBLE_EQ(filter_same_bin_as_target_freq_2.filter_magnitude(filter_samples), filter_at_target_freq.filter_magnitude(filter_samples));

    // Check that bins off target frequency bin report lower magnitude, as expected with a bandpass filter and a bell curve response.
    GoertzelFilter filter_one_off_center_forward((bin_of_target_frequency * bin_width) + (bin_width * 0.5), block_size, sample_rate, 0);
    ASSERT_LT(filter_one_off_center_forward.filter_magnitude(filter_samples), filter_at_target_freq.filter_magnitude(filter_samples));
}

}






















