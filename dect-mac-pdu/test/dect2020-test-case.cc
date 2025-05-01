#include "dect2020-test-case.h"

namespace ns3 {

Dect2020TestCase::Dect2020TestCase() : TestCase("Test")
{
}

Dect2020TestCase::~Dect2020TestCase()
{
}

void
Dect2020TestCase::DoSetup()
{
}

void
Dect2020TestCase::DoRun()
{
    NS_TEST_ASSERT_MSG_EQ(true, true, "[ERROR]");
}

} // namespace ns3