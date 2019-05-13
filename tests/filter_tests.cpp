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


// Can detect a frequency using the filter. Relies on LibSndFileTests
// can_create_and_save_wav_file for the test data.
TEST(AudioFilterTests, can_detect_frequency_from_single_frequency_source) {
    SNDFILE* file;
    SF_INFO sfinfo;
    vector<double> filter_samples;
    const double sample_rate = 48000.0;
    const double block_size = 320.0;
    const double target_frequency = 350.0;

    sfinfo.samplerate	= static_cast<int>(sample_rate);
    sfinfo.frames		= (static_cast<int>(sample_rate) * 4);
    sfinfo.channels		= 1;
    sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24);

    file = sf_open ("../data/sine_350hz.wav", SFM_READ, &sfinfo);

    ASSERT_NE(nullptr, file); // Check the file opened.

    sf_count_t samples_read = 0;
    double samples[1000];

    samples_read = sf_read_double(file, samples, 1000);
    filter_samples.assign(samples, samples+1000);


    // Test detection at target frequency
    GoertzelFilter filter_at_target_freq(target_frequency, block_size, sample_rate, 0);
    ASSERT_TRUE(filter_at_target_freq.detect_frequency(filter_samples));
}

}






















