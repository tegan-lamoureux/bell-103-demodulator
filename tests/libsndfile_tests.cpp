#include "gtest/gtest.h"

#include <sndfile.h>
#include <cmath>

using namespace std;

namespace {


// Test using basic library objects to see if we have access 
// and can link correctly.
TEST(LibSndFileTests, can_use_libsndfile) {
	bool got_this_far = true;
	
	SNDFILE* file;
	SF_INFO sfinfo;

	ASSERT_TRUE(got_this_far);
}


// Test opening a WAV file.
TEST(LibSndFileTests, can_open_wav_file) {
	SNDFILE* file;
	SF_INFO sfinfo;
	
    sfinfo.samplerate	= 48000;
    sfinfo.frames		= (48000 * 4);
    sfinfo.channels		= 1;
	sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24);

	file = sf_open ("../data/lamoureux.wav", SFM_READ, &sfinfo);

	ASSERT_NE(nullptr, file); // Check the file opened.
	ASSERT_EQ(0, sf_close(file)); // Check the file closed.
}


// Test saving a wav file
// (Example taken from: https://github.com/erikd/libsndfile/blob/master/examples/make_sine.c)
TEST(LibSndFileTests, can_create_and_save_wav_file) {
    const double pi = 3.14159265358979323846264338;
    const long SAMPLE_RATE = 48000;
    const long SAMPLE_COUNT = (SAMPLE_RATE * 4);	/* 4 seconds */
    const long AMPLITUDE = (1.0 * 0x7F000000);
    const double FREQ = (2025.0 / SAMPLE_RATE);

    SNDFILE	*file ;
    SF_INFO	sfinfo ;
    int		k ;
    int	*buffer ;

    buffer = static_cast<int*>(malloc (2 * SAMPLE_COUNT * sizeof (int)));

    ASSERT_NE(nullptr, buffer);

    memset(&sfinfo, 0, sizeof (sfinfo)) ;

    sfinfo.samplerate	= SAMPLE_RATE ;
    sfinfo.frames		= SAMPLE_COUNT ;
    sfinfo.channels		= 1 ;
    sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24) ;

    file = sf_open ("2025.wav", SFM_WRITE, &sfinfo);

    ASSERT_NE(nullptr, file);

    for (k = 0 ; k < SAMPLE_COUNT ; k++) {
        buffer [k] = AMPLITUDE * sin (FREQ * 2 * k * pi) ;
    }

    long bytes_written = sf_write_int (file, buffer, sfinfo.channels * SAMPLE_COUNT);

    ASSERT_EQ(bytes_written, sfinfo.channels * SAMPLE_COUNT);

    sf_close (file) ;
    free (buffer) ;
}

}






















