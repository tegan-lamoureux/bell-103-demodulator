### Bell 103 Demodulator
#### Tegan Lamoureux; CS410P Homework 1

A demodulator for a subset of the Bell 103 modem protocol. 

#### Process
Starting with https://www.embedded.com/design/configurable-systems/4024443/The-Goertzel-Algorithm, I implemented a basic filter. I had trouble with an integer parameter K, as it forced my mark and space frequencies into the same bin, and I was getting the same magnitude. With help, I realized I didn't need to force an integer value here, and left the floating point value. This gave me much better magnitude detection, and I was able to detect the mark and space frequencies message effectively.

I used cmake to set up the build environment for this project, and used the google test framework to unit test as I developed, following a test driven development process.

#### Building
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
