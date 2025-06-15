#ifndef DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_PAYLOAD_INSERTION_TEST_CASE_H
#define DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_PAYLOAD_INSERTION_TEST_CASE_H

#include "ns3/test.h"

namespace ns3
{

class Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase : public TestCase
{
	public:
		Dect2020MacMultiplexingHeaderTypeCPayloadInsertionTestCase();

		virtual void DoSetup();
		virtual void DoRun();
};

} // namespace ns3

#endif /* DECT2020_MAC_MULTIPLEXING_HEADER_TYPE_C_PAYLOAD_INSERTION_TEST_CASE_H */
