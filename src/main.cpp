#include <iostream>
#include <vector>
#include <string>
#include <sndfile.h>

#include "goertzel_filter.h"

using namespace std;

/**
 * @brief load_mono_wave_file
 * @param filename
 * @param sample_rate
 * @return
 */
vector<double> load_mono_wave_file(string filename, double sample_rate);

/**
 * @brief demodulate_signal
 * @param buffer_to_decode
 * @param sample_rate
 * @return
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
    SNDFILE* file;
    SF_INFO sfinfo;
    vector<double> filter_samples;

    sfinfo.samplerate = static_cast<int>(sample_rate);
    sfinfo.frames     = (static_cast<int>(sample_rate) * 4);
    sfinfo.channels   = 1;
    sfinfo.format     = (SF_FORMAT_WAV | SF_FORMAT_PCM_24);

    file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        cout << "File read error." << endl;
        exit(1);
    }

    sf_count_t samples_read = 0;
    vector<double> samples_read_buffer;
    samples_read_buffer.resize(128);

    samples_read = sf_read_double(file, samples_read_buffer.data(), 128);

    while (samples_read > 0) {
        filter_samples.insert(filter_samples.end(), samples_read_buffer.begin(), samples_read_buffer.end());
        samples_read = sf_read_double(file, samples_read_buffer.data(), 128);
    }

    return filter_samples;
}



string demodulate_signal(vector<double>& buffer_to_decode, double sample_rate) {
    const int block_size = 160;

    GoertzelFilter mark(2225, block_size, sample_rate, 0);
    GoertzelFilter space(2025, block_size, sample_rate, 0);

    string decoded_message = "";
    unsigned int bit_number = 0;
    uint8_t decoded_byte = 0;

    // Move along the main sample vector and take subsamples of 160 each, then run
    // mark and space detection, accumulating message.
    for (int location = 0; location < buffer_to_decode.size(); location += 160) {
        if (bit_number == 0 || bit_number == 9) {
            bit_number++;
            continue; // Hack for start/stop bit.
        }

        if (bit_number >= 9) {
            decoded_message += static_cast<char>(decoded_byte);
            bit_number = 0;
            decoded_byte = 0;
            bit_number++;
            continue;
        }

        auto subsample_start = buffer_to_decode.begin() + location;
        auto subsample_end = buffer_to_decode.begin() + location + 160;

        vector<double> subsample(subsample_start, subsample_end);

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
