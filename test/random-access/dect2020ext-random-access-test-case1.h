#ifndef DECT2020_EXT_RANDOM_ACCESS_TEST_CASE1_H
#define DECT2020_EXT_RANDOM_ACCESS_TEST_CASE1_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020ExtRandomAccessTestCase1 : public TestCase
{
	public:
		Dect2020ExtRandomAccessTestCase1();
		~Dect2020ExtRandomAccessTestCase1() override;

		void DoSetup() override;
		void DoRun() override;
};

} // namespace ns3

#endif // DECT2020_EXT_RANDOM_ACCESS_TEST_CASE1_H

