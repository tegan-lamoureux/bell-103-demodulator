#include <iostream>
#include <vector>
#include <string>
#include <sndfile.h>

#include "goertzel_filter.h"

using namespace std;

/** @brief load_mono_wave_file loads a vector with samples in floating point format. Closely follows some
 *         of the examples at: https://github.com/erikd/libsndfile/tree/master/examples
 *
 *  @param[in] filename of the wav file to load.
 *  @param[in] sample_rate of the wav file.
 *
 *  @returns sample buffer.
 */
vector<double> load_mono_wave_file(string filename, double sample_rate);

/** @brief demodulate_signal returns an ascii representation of the input signal using the bell 103 modem protocol.
 *
 *  @param[in] buffer_to_decode floating point samples to process.
 *  @param[in] sample_rate of the buffer, in Hz.
 *
 *  @returns the ascii representation of the signal.
 */
string demodulate_signal(vector<double>& buffer_to_decode, double sample_rate);


int main(int argc, char* argv[])
{
    if (argc != 3 || atof(argv[2]) == 0.0) {
        cout << "Usage: " << argv[0] << " file_to_decode.wav sample_rate_in_Hz" << endl;
        return 1;
    }

    // Load parameters from command line.
    double sample_rate = atof(argv[2]);
    string filename = argv[1];

    // Load wave file samples into buffer.
    vector<double> buffer_to_decode = load_mono_wave_file(filename, sample_rate);

    // Demodulate and print the message.
    cout << demodulate_signal(buffer_to_decode, sample_rate) << endl;

    return 0;
}



vector<double> load_mono_wave_file(string filename, double sample_rate) {
    vector<double> filter_samples;
    SNDFILE* file;
    SF_INFO sfinfo;

    // Wave file settings.
    sfinfo.samplerate = static_cast<int>(sample_rate);
    sfinfo.frames     = (static_cast<int>(sample_rate) * 4);
    sfinfo.channels   = 1;
    sfinfo.format     = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);

    // Open and check for error.
    file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        cout << "File read error." << endl;
        exit(1);
    }

    // File processing variables.
    sf_count_t samples_read = 0;
    vector<double> samples_read_buffer;
    samples_read_buffer.resize(128);

    // Load samples from file in chunks of 128 bytes and append to output vector.
    samples_read = sf_read_double(file, samples_read_buffer.data(), 128);
    while (samples_read > 0) {
        filter_samples.insert(filter_samples.end(), samples_read_buffer.begin(), samples_read_buffer.end());
        samples_read = sf_read_double(file, samples_read_buffer.data(), 128);
    }

    return filter_samples;
}



string demodulate_signal(vector<double>& buffer_to_decode, double sample_rate) {
    const int block_size = 160; // Fixed block size for 300 baud @ 48kHz sample rate. (48k / 300 = 160)

    // Mark and space frequency filters @ 2225 Hz and 2025 Hz respectively.
    GoertzelFilter mark(2225, block_size, sample_rate, 0);
    GoertzelFilter space(2025, block_size, sample_rate, 0);

    // Byte-processing variables.
    string decoded_message = "";
    unsigned int bit_number = 0;
    uint8_t decoded_byte = 0;

    // Move along the main sample vector and take subsamples of 160 each, then run
    // mark and space detection, accumulating message bits.
    for (int location = 0; location < buffer_to_decode.size(); location += 160) {
        // Skip start and stop bits (this is easy/hack-ish since our data is so nice, it would
        // not work in the real world).
        if (bit_number == 0 || bit_number == 9) {
            bit_number++;
            continue;
        }

        // Append output data and reset after accumulating 8 bits.
        if (bit_number >= 9) {
            decoded_message += static_cast<char>(decoded_byte);
            bit_number = 0;
            decoded_byte = 0;
            bit_number++;
            continue;
        }

        // Define subsample at our block size.
        auto subsample_start = buffer_to_decode.begin() + location;
        auto subsample_end = buffer_to_decode.begin() + location + block_size;
        vector<double> subsample(subsample_start, subsample_end);

        // Run detection.
        if (mark.filter_magnitude(subsample) > space.filter_magnitude(subsample)) {
            decoded_byte = decoded_byte | (1 << (bit_number - 1));
        }
        else {
            decoded_byte = decoded_byte & ~(1 << (bit_number - 1));
        }

        bit_number++;
    }

    return decoded_message;
}
