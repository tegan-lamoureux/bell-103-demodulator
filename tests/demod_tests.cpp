#include "gtest/gtest.h"

using namespace std;

namespace {

class ModemTests : public testing::Test {
protected:
    ModemTests() {

    }

    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};


TEST_F(ModemTests, is_true_true) {
    ASSERT_TRUE(true);
}

}






















