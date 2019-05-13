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

    // 44.1 kHz and block size of 320 give bin width of 137 Hz.

    // Test at target frequency
	GoertzelFilter filter(target_frequency, block_size, sample_rate, 0);
    ASSERT_TRUE(filter.detect_frequency(filter_samples));

	// Prove bin width. (Create two filters in same bin and check output. Also check all in target frequenct bin.)
	const double bin_width = (sample_rate / block_size);
	const int bin_of_target_frequency = static_cast<int>((target_frequency / bin_width) + 0.5);

	GoertzelFilter filter_same_bin1(((bin_of_target_frequency + 1) * bin_width) - (bin_width * 0.1), block_size, sample_rate, 0); // Filter at end of target freq's bin.
	GoertzelFilter filter_same_bin2(((bin_of_target_frequency + 1) * bin_width) - (bin_width * 0.9), block_size, sample_rate, 0); // Filter at start of target freq's bin.

	ASSERT_DOUBLE_EQ(filter_same_bin1.filter_magnitude(filter_samples), filter_same_bin2.filter_magnitude(filter_samples));
	ASSERT_DOUBLE_EQ(filter_same_bin1.filter_magnitude(filter_samples), filter.filter_magnitude(filter_samples));
	ASSERT_DOUBLE_EQ(filter_same_bin2.filter_magnitude(filter_samples), filter.filter_magnitude(filter_samples));

    // Test one bin off center.
    GoertzelFilter filter2(200, 320, 44100, 0);
    ASSERT_TRUE(filter2.detect_frequency(filter_samples));

    // Test two bins off center.
    GoertzelFilter filter3(60, 320, 44100, 0);
    ASSERT_TRUE(filter3.detect_frequency(filter_samples));
}

}






















