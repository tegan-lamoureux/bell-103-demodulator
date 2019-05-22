### Bell 103 Demodulator

A demodulator for a subset of the Bell 103 modem protocol.

#### Process
Starting with https://www.embedded.com/design/configurable-systems/4024443/The-Goertzel-Algorithm, I implemented a basic single-bin DFT filter. I had trouble with an integer parameter K, as it forced my mark and space frequencies into the same bin, and I was getting the same magnitude for both. With help, I realized I didn't need to force an integer value here, and left the floating point value which allowed my 'bin' to be centered at my target frequency. This gave me much better magnitude detection, and I was able to differentiate between the mark and space frequencies effectively.

I used cmake to set up the build environment for this project, and used the google test framework to unit test as I developed, following a test driven development process.

#### Using It
Dependencies:
* cmake
* libsndfile development libraries
* These are available on most platforms as libsndfile-devel, or libsndfile1-devel
* gtest and unit tests are automatically downloaded and built as part of the cmake setup process.

Building:
* Clone to a local repository.
* In the repository's main directory, make a build directory with `mkdir build`.
* Inside the build directory, run cmake to generate build files and targets: `cd build && cmake ../`
* Build application and unit tests with: `make`

#### To-Do
This is very specific to the input file. It depends on sanitized data and perfectly spaced bits at 300 baud with no padding or lead in. It won't sync up from the carrier tone to the start bit. It could be much more robust in this regard.

Performance wise, floating point could be converted to fixed point notation integer math to speed up calculations. Inline GCC assembly utilizing SSE/AVX2 could be used to perform vectored calculations, giving a possibly 20-fold increase in performance considering block sizes of 160 would fit nicely into 8 lane vectors.
