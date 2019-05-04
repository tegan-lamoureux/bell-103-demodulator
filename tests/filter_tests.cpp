#include "gtest/gtest.h"

#include "audio_filters.h"
#include <sndfile.h>
#include <string>
#include <cmath>

using namespace std;

namespace {

void create_wav_file(double left_frequency, double right_frequency, std::string filename) {
    const double pi = 3.14159265358979323846264338;
    const long SAMPLE_RATE = 44100;
    const long SAMPLE_COUNT = (SAMPLE_RATE * 4);	/* 4 seconds */
    const long AMPLITUDE = (1.0 * 0x7F000000);
    const double LEFT_FREQ = (left_frequency / SAMPLE_RATE);
    const double RIGHT_FREQ = (right_frequency / SAMPLE_RATE);

    SNDFILE	*file ;
    SF_INFO	sfinfo ;
    int		k ;
    int	*buffer ;

    buffer = static_cast<int*>(malloc (2 * SAMPLE_COUNT * sizeof (int)));

    ASSERT_NE(nullptr, buffer);

    memset(&sfinfo, 0, sizeof (sfinfo)) ;

    sfinfo.samplerate	= SAMPLE_RATE ;
    sfinfo.frames		= SAMPLE_COUNT ;
    sfinfo.channels		= 2 ;
    sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24) ;

    file = sf_open (filename.c_str(), SFM_WRITE, &sfinfo);

    ASSERT_NE(nullptr, file);
    ASSERT_TRUE(sfinfo.channels == 1 || sfinfo.channels == 2);

    if (sfinfo.channels == 1) {
        for (k = 0 ; k < SAMPLE_COUNT ; k++) {
            buffer [k] = AMPLITUDE * sin (LEFT_FREQ * 2 * k * pi) ;
        }
    }
    else if (sfinfo.channels == 2) {
        for (k = 0 ; k < SAMPLE_COUNT ; k++) {
            buffer [2 * k] = AMPLITUDE * sin (LEFT_FREQ * 2 * k * pi) ;
            buffer [2 * k + 1] = AMPLITUDE * sin (RIGHT_FREQ * 2 * k * pi) ;
        }
    }

    long bytes_written = sf_write_int (file, buffer, sfinfo.channels * SAMPLE_COUNT);

    ASSERT_EQ(bytes_written, sfinfo.channels * SAMPLE_COUNT);

    sf_close (file) ;
    free (buffer) ;
}

// Can instantiate audio filter test.
TEST(AudioFilterTests, can_use_libsndfile) {
    bool got_this_far = true;

    AudioFilters filters;

    ASSERT_TRUE(got_this_far);
}


// Can detect a .
TEST(AudioFilterTests, can_detect_frequency_from_single_frequency_source) {
    // Make a 350 hz sine wave wave file to detect.

}

}






















