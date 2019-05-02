#include "gtest/gtest.h"

#include <sndfile.h>

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
	
	sfinfo.samplerate	= 44100;
	sfinfo.frames		= (44100 * 4);
	sfinfo.channels		= 2;
	sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24);

	file = sf_open ("../data/lamoureux.wav", SFM_READ, &sfinfo);

	ASSERT_NE(nullptr, file); // Check the file opened.
	ASSERT_EQ(0, sf_close(file)); // Check the file closed.
}

}






















